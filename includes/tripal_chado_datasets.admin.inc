<?php

//TODO:  these functions were the admin form, but i would like to move them to say a curation form, and use "admin" to refer to administering this module itself.

/**
 * @file
 * Admin page callbacks for the datasets module.
 * Form builder; Admin interface to approve/reject the organisms add by the
 * co-ordinators.
 */

/**
 * @hook_form()
 * Requested Organism form with options Approve/Reject
 * Note: the form and form_state parameters are ignored here.
 *
 * @param $id = ds_request_project.id
 */
function tripal_chado_datasets_organism_approval($form, &$form_state, $id) {
  $header = [];
  drupal_add_js(drupal_get_path('module', 'tripal_chado_datasets') . '/js/organism_approval_status.js');

  // sql here
  $data = db_select('ds_request_project', 'rp')
    ->fields('rp', [
      'id',
      'fullname',
      'email',
      'genus',
      'species',
      'ncbi_taxid',
      'common_name',
      'genome_assembly_hosted',
      'is_ncbi_submitted',
      'is_assembly',
      'involved_in_generation',
      'description',
      'status',
    ])
    ->condition('rp.id', $id, '=')
    ->execute()->fetchObject();
  // Now build the form array
  $form = [];

  $form['datasets_table'] = [
    '#tree' => TRUE, // Need this for easy submit processing
    '#header' => $header, // We trick Drupal to pass our $header to theme here
    '#theme' => 'tripal_chado_datasets_organism_approval',
  ];
  $form['datasets_table']['fullname'] = [
    '#type' => 'value',
    '#title' => t('Name'),
    '#value' => $data->fullname,
  ];
  $form['datasets_table']['email'] = [
    '#type' => 'value',
    '#value' => $data->email,
  ];
  $form['datasets_table']['genus'] = [
    '#type' => 'value',
    '#value' => $data->genus,
  ];
  $form['datasets_table']['species'] = [
    '#type' => 'value',
    '#value' => $data->species,
  ];
  $form['datasets_table']['ncbi_taxid'] = [
    '#type' => 'value',
    '#value' => $data->ncbi_taxid,
  ];
  $form['datasets_table']['common_name'] = [
    '#type' => 'value',
    '#value' => $data->common_name,
  ];
  $form['datasets_table']['genome_assembly_hosted'] = [
    '#type' => 'value',
    '#value' => $data->genome_assembly_hosted,
  ];

  $form['datasets_table']['is_ncbi_submitted'] = [
    '#type' => 'value',
    '#value' => $data->is_ncbi_submitted,
  ];

  $form['datasets_table']['is_assembly'] = [
    '#type' => 'value',
    '#value' => $data->is_assembly,
  ];

  $form['datasets_table']['involved_in_generation'] = [
    '#type' => 'value',
    '#value' => $data->involved_in_generation,
  ];

  $form['datasets_table']['description'] = [
    '#type' => 'value',
    '#title' => 'Description',
    '#value' => $data->description,
  ];

  $status = '';

  if ($data->status == 1) {
    $status = 'Approved';
  }
  elseif ($data->status == 0) {
    $status = 'Pending';
  }
  else {
    $status = 'Rejected';
  }

  $form['datasets_table']['status'] = [
    '#type' => 'radios',
    '#default_value' => $data->status,
    '#options' => [t('Reject'), t('Approve')],
    '#attributes' => ['onClick' => 'statusclick(this.value, "' . $data->genus . ' ' . $data->species . '")'],
  ];
  $form['datasets_table']['id'] = [
    '#type' => 'value',
    '#value' => $data->id,
  ];

  // Submit function
  $form['datasets_table']['submit'] = [
    '#type' => 'submit',
    '#name' => 'submit',
    '#value' => t('Save'),
  ];

  return $form;
}

/**
 * @hook_form_submit()
 *
 * @sends an email to user regarding login credentials to the web apollo site.
 */
function tripal_chado_datasets_organism_approval_submit($form, &$form_state) {
  $values = $form_state['values']['datasets_table'];
  $status = $values['status'];
  $id = $values['id'];
  $to = $values['email'];
  $from = FROM_ADDRESS;

  $status_res = db_select('ds_request_project', 'rq')
    ->fields('rq', ['status'])
    ->condition('rq.id', $id, '=')
    ->execute()->fetchField();

  if ($status_res != $status && $status == 1) {
    $genus = strtolower($values['genus']);
    $species = strtolower($values['species']);

    /** Changing the status to TRUE **/
    db_update('ds_request_project')
      ->fields(['status' => $status])
      ->condition('id', $id, '=')
      ->execute();
    //   Send email to the user about the login credentials to the web apollo site
    $user_email_sent = drupal_mail('tripal_chado_datasets', 'request_project_approved_email', $to, language_default(), $values, $from, TRUE);

    // $is_species_exists = db_select('chado.organism', 'o')->fields('o', array('organism_id'))->condition('o.genus', $genus, 'LIKE')->condition('o.species', $species, 'LIKE')->execute()->fetchField();
    $sql = "SELECT o.organism_id  
          FROM chado.organism o 
          WHERE o.genus LIKE :genus and  o.species LIKE :species";
    $args = [':genus' => $genus, ':species' => $species];
    $is_species_exists = chado_query($sql, $args)->fetchObject();

    if (!empty($is_species_exists)) {
      drupal_set_message(t('This species %genus %species already exists in database .'), [
        '%genus' => $genus,
        '%species' => $species,
      ], $type = 'status');
      return;
    }


    /************************************************************
     ***********************************************************
     * INSERT the approved request project 'organism' into  chado .*/

    //Tripal 2 way

    if (function_exists('chado_get_id_from_nid')) {

      $node = new stdClass();
      $node->type = "chado_organism";
      $node->language = "und";
      $node->title = $values['genus'] . " " . $values['species'];
      $node->uid = 1;
      $node->status = 0;
      $node->created = time();
      $node->changed = time();
      $node->promote = 0;


      $node->genus = trim($values['genus']);
      $node->species = trim($values['species']);
      $node->abbreviation = $node->genus[0] . " . " . $node->species; //trim($data->abbreviation);
      $node->common_name = trim($values['common_name']);
      $node->description = trim($values['description']);

      node_save($node);

      $organism_id = $node->organism->organism_id;
      $check_org_id = chado_get_id_from_nid('organism', $node->nid);
      if (!$check_org_id) {
        $record = new stdClass();
        $record->nid = $node->nid;
        $record->vid = $node->vid;
        $record->organism_id = $organism_id;

      }
    }

    //Tripal 3
    else {

      $genus = trim($values['genus']);
      $species = trim($values['species']);
      $abbreviation = $genus[0] . ". " . $species;
      $common_name = trim($values['common_name']);
      $description = trim($values['description']);

      $values = [
        'abbreviation' => $abbreviation,
        'genus' => $genus,
        'species' => $species,
        'common_name' => $common_name,
        'comment' => $description,
      ];
      $options = [];
      $record = chado_insert_record('organism', $values, $options);

      if ($record) {
        tripal_set_message(t('Organism created.  Please run publish for organism entities.'), TRIPAL_NOTICE);

      }
      else {
        tripal_set_message(t('Error creating organism!', TRIPAL_ERROR));
      }
    }


    /*******************************************************************************************
     *******************************************************************************************/
    //Displaying status message on the site
    $status = ($status == 1) ? 'Approved' : 'Rejected';
    drupal_set_message(t('%genus %species status was changed to %status'), [
      '%genus' => $values['genus'],
      '%species' => $values['species'],
      '%status' => $status,
    ], $type = 'status');

  }
  elseif ($status == 0) {
    db_update('ds_request_project')// Table name no longer needs {}
    ->fields(['status' => $status])
      ->condition('id', $id, '=')
      ->execute();
    /*
        $query = db_select('organism', 'o');
        // Select fields
        $query->fields("o", array("organism_id"));
        // Condition
        $query->condition("genus", $values['genus']);
        // Execute query
        $result = $query->execute()->fetchObject();

        $oid = $result->organism_id;
        $node_id = chado_get_nid_from_id('organism', $oid);
        // Deletinge node related tables, chado_organism and chado.organism table
        db_delete('node')
          ->condition('nid', $node_id, '=')
          ->execute();
        db_delete('node_revision')
          ->condition('nid', $node_id, '=')
          ->execute();
        db_delete('history')
          ->condition('nid', $node_id, '=')
          ->execute();
        db_delete('node_access')
          ->condition('nid', $node_id, '=')
          ->execute();
        db_delete('chado_organism')
          ->condition('nid', $node_id, '=')
          ->execute();
        db_delete('organism')
          ->condition('organism_id', $oid, '=')
          ->execute();
         */

    //Also removing rejected organism from the settings form - admin/structure/webapollo/settings


    //    $web_apollo_organism = variable_get('web_apollo_organism');
    //    $organism_shortcut = strtolower(substr($values['genus'], 0, 3) . substr($values['species'], 0, 3));
    //    $remove_str = $organism_shortcut . '|' . $values['genus'] . ' ' . $values['species'] . '|0';
    //    $replace_organism = str_replace($remove_str, '', $web_apollo_organism);
    //    variable_set('web_apollo_organism', $replace_organism);

    drupal_set_message(t('%genus %species has been rejected by  the admin due to some reasons.'), [
      '%genus' => $values['genus'],
      '%species' => $values['species'],
    ], $type = 'status');
  }

  $form_state['redirect'] = 'admin/structure/datasets';
  return;
}


/**
 * Form at admin/structure/datasets/report
 */
function tripal_chado_datasets_report($form, &$form_state) {
  $organism_options = chado_get_organism_select_options(FALSE);

  $organism_selected = 0;

  if (!empty($form_state['input']['organism'])) {
    $organism_selected = $form_state['input']['organism'];
  }
  elseif (isset($_GET['organism']) && !empty($_GET['organism'])) {
    $organism_selected = $_GET['organism'];
  }

  $_GET['q'] = 'admin/structure/datasets/report';

  $form['#attached']['css'] = [
    drupal_get_path('module', 'tripal_chado_datasets') . '/css/report.css',
  ];
  $form['header_text'] = [
    '#markup' => t('<div id="text">Below report is for user\'s who submitted their datasets for Assembly, gene and mapped.</div>'),
  ];

  $form['organism'] = [
    '#type' => 'select',
    '#title' => t('Organism'),
    '#options' => $organism_options,
    '#default_value' => $organism_selected,
    '#description' => t('If no organism is selected it shows by default all organism data.'),
    '#ajax' => [
      'callback' => 'tripal_chado_datasets_report_callback',
      'wrapper' => 'report',
    ],
  ];

  $rows = [];
  if (isset($organism_selected)) {
    $rows = tripal_chado_datasets_list_all_submissions($organism_selected);
  }
  else {
    $rows = tripal_chado_datasets_list_all_submissions();
  }

  // Prepare table header
  $header = [
    ["data" => t('Name'), 'field' => 'name'],
    ["data" => t('Email'), 'field' => 'email'],
    ["data" => t('Organism'), 'field' => 'organism', 'org' => 'vj'],
    ["data" => t('Type'), 'field' => 'type'],
    ["data" => t('Loggedin User'), 'field' => 'uid'],
    ["data" => t('Created'), 'field' => 'created', 'sort' => 'desc'],
    [],
  ];

  $order = tablesort_get_order($header);
  $sort = tablesort_get_sort($header);
  $sql = $order['sql'];
  if ($sort == 'desc') {
    usort($rows, function ($a, $b) use ($sql) {
      $order_fields = [
        'name' => 0,
        'email' => 1,
        'organism' => 2,
        'type' => 3,
        'created' => 5,
      ];
      $okey = $order_fields[$sql];
      return strtolower($a[$okey]) > strtolower($b[$okey]) ? -1 : 1;
    });

  }
  if ($sort == 'asc') {
    usort($rows, function ($a, $b) use ($sql) {
      $order_fields = [
        'name' => 0,
        'email' => 1,
        'organism' => 2,
        'type' => 3,
        'created' => 5,
      ];
      $okey = $order_fields[$sql];
      return strtolower($a[$okey]) < strtolower($b[$okey]) ? -1 : 1;
    });
  }

  //   echo "<prE>";   print_r($rows);echo "</pre>";
  $per_page = 25;
  // Initialize the pager
  $current_page = pager_default_initialize(count($rows), $per_page);

  // Split your list into page sized chunks
  $chunks = array_chunk($rows, $per_page, TRUE);

  // Show the appropriate items from the list
  //$output = theme('table', array('header' => $header, 'rows' => $chunks[$current_page]));
  // Show the pager
  //$output .= theme('pager', array('quantity',count($rows)));

  $output = tripal_chado_datasets_theme_datasets_table(
    [
      "header" => $header,
      "rows" => $chunks[$current_page],
      "attributes" => ['org' => $organism_selected],
      "sticky" => TRUE,
      // Table header will be sticky
      "caption" => "",
      "colgroups" => [],
      "empty" => t("Table has no rows!")
      // The message to be displayed if table is empty
    ]
  );
  $parameters = ['organism', $organism_selected];
  $pager = [];
  $pager['quantity'] = count($rows);
  $output .= theme('pager', [
    'quantity' => count($rows),
    'parameters' => ['organism' => $organism_selected],
  ]);
  //   $output .= theme('pager', array('quantity' => count($rows));

  $form['result'] = [
    '#prefix' => '<div id="report">',
    '#suffix' => '</div>',
    '#type' => 'fieldset',
  ];

  if (isset($form_state['input']['organism'])) {
    $form['result']['report']['#suffix'] = $output;
  }
  else {
    $form['result']['report']['#suffix'] = $output;
  }

  return $form;
}

function tripal_chado_datasets_report_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

/**
 * Build a table of of all submissions, or of submissions for a given organism
 * if $organism is supplied.
 *
 * @param null $organism
 *  The chado record organism_id.
 *
 * @return array
 */

function tripal_chado_datasets_list_all_submissions($organism = NULL) {

  $sort = 'DESC';
  $order = 'name';

  $organism_store = [];

  $query = db_select("ds_submissions", "c");
  $query->fields("c", [
    "oa_id",
    "name",
    "email",
    "organism",
    "uid",
    "created",
    "dataset_type",
  ]);
  if (!empty($organism)) {
    $query->condition('organism', $organism);
  }
  $query->orderBy($order, $sort);
  $result_oa = $query->execute()->fetchAll();
  $rows = [];
  $i = 0;

  // Looping for filling the table rows  
  foreach ($result_oa as $key => $data) {
    // Fill the table rows
    $rows[$i][] = $data->name;
    $rows[$i][] = $data->email;

    $organism_id = $data->organism;

    //If this is a new organism, build something to display to the sure.
    if (isset($organism_store[$organism_id])) {
      $organism_string = $organism_store[$organism_id];
    }
    else {
      $organism_data = chado_get_organism(['organism_id' => $organism_id]);
      $organism_string = $organism_data->abbreviation . ', (' . $organism_data->common_name . ')';
      $organism_store[$organism_id] = $organism_string;
    }
    $rows[$i][] = $organism_string;
    $rows[$i][] = $data->dataset_type;
    $user = user_load($data->uid);
    $rows[$i][] = (!empty($user)) ? $user->name : '';
    $rows[$i][] = date('Y-m-d', $data->created);
    $rows[$i][] = l(t('View'), '/datasets/' . $data->oa_id . '/view', ['attributes' => ['target' => '_blank']]) . ' ' . l(t('Delete'), '/datasets_og/' . $data->oa_id . '/delete', ['attributes' => ['target' => '_blank']]);
    $i++;
  }
  return $rows;
}

/**
 * TODO: is this form ever used?
 */
function tripal_chado_datasets_md_delete_form($form, &$form_state, $item) {
  $form['#item'] = $item;
  // Note confirm_form() can be used here, but I prefer to use my own for styling purposes
  $form['header'] = [
    '#markup' => t('Are you sure you wish to delete the item?'),
    '#prefix' => '',
    '#suffix' => '<br />',
  ];
  $form['warning'] = [
    '#markup' => t('Warning, this action cannot be undone<br><br>'),
    '#prefix' => '',
    '#suffix' => '',
  ];
  $form['delete_button'] = [
    '#type' => 'submit',
    '#value' => t('Delete item'),
  ];
  return $form;
}

function tripal_chado_datasets_md_delete_form_submit($form, &$form_state) {
  if ($form_state['values']['op'] == $form_state['values']['delete_button']) {
    $item = $form['#item'];
    db_delete('ds_mapped_dataset')
      ->condition('id', $item)
      ->execute();
  }
  $form_state['redirect'] = 'admin/structure/datasets/report';
  drupal_set_message(t('The item has been deleted'));
}

function tripal_chado_datasets_report_callback($form, $form_state) {
  return $form['result'];
}


function tripal_chado_datasets_information_import() {
  $output = "";

  //Leaving in a single one of these, presumably as an example.

  //  db_query("INSERT INTO {public.ds_organism_assembly} (oa_id, uid, name, email, organism, common_name, description, organism_image_url, is_curate_assembly, is_same, manual_curation_name, manual_curation_email, need_assistance, reason, time_from, time_to, no_timeframe, assembly_name, assembly_version, assembly_accession, assembly_method, is_publish, publish_field_data, other_notes, geo_location, tissues_located, gender, other_gender, data_source_strain, data_source_notes, data_source_seqplatform, data_source_url, filename, md5sum, sha512, created) VALUES (1, 752, 'Hugh Robertson', 'hughrobe@life.illinois.edu', 'cepcin', 'Wheat stem sawfly', 'will be provided soon', '', 'Yes', 1, 'Hugh Robertson', 'hughrobe@life.illinois.edu', 'No', NULL, 0, 0, 1, 'Cephus_cinctus, Ccin1', '1', 'GCA_000341935.1', 'SOAPdenovo  v. 2.04', 'No', 'yes', '', 'USA:Montana:Amsterdam-Churchill', 'whole individual', 'Male', NULL, '', '', 'Illumina GAIIx/Hi-Seq 2000', '', 'GCF_000341935.1_Ccin1_genomic.fna.gz', '685432a8ab91bafe73267ebf0ea86968', NULL, 1470157024)");


  // Select table ds_gene_prediction
  $query = db_select("ds_gene_prediction", "gp");
  // Select fields
  $query->fields("gp");

  // Execute query
  $result = $query->execute();

  // Looping for filling the table rows  
  while ($row = $result->fetchObject()) {
    $data['uid'] = $row->uid;
    $data['organism'] = $row->organism;
    $data['program'] = $row->program;
    $data['version'] = $row->version;
    $data['additional_info'] = $row->additional_info;
    $data['methods_citation'] = $row->other_methods;
    $data['name'] = $row->name;
    $data['email'] = $row->email;
    $data['affiliation'] = $row->affiliation;
    $data['dataset_name'] = $row->gene_name;
    $data['dataset_version'] = $row->gene_version;
    $data['gene_set_descriptive_track'] = $row->descriptive_track;
    $data['is_ogs'] = $row->is_ogs;
    $data['reason'] = $row->reason;
    $data['is_download'] = $row->is_download;
    $data['filename'] = $row->filename;
    $data['md5sum'] = $row->md5sum;
    $data['sha512'] = $row->sha512;
    $data['created'] = $row->created;
    $data['dataset_type'] = 'Gene prediction';
    $names = implode(', ', array_keys($data));
    $values = implode("', '", array_values($data));
    db_query("insert into {public.ds_submissions}($names) values('$values')");
  }

  // Select table ds_mapped_dataset
  unset($data);
  $query = db_select("ds_mapped_dataset", "gp");
  // Select fields
  $query->fields("gp");

  // Execute query
  $result = $query->execute();

  // Looping for filling the table rows  
  while ($row = $result->fetchObject()) {
    $data['uid'] = $row->uid;
    $data['organism'] = $row->organism;
    $data['name'] = $row->name;
    $data['email'] = $row->email;
    $data['affiliation'] = $row->affiliation;
    $data['mapped_dataset_geo_location'] = $row->geo_location;
    $data['mapped_dataset_tissues_located'] = $row->tissues_located;
    $data['mapped_gender'] = $row->gender;
    $data['other_gender'] = $row->other_gender;
    $data['sequence_platform'] = $row->sequence_platform;
    $data['mapped_dataset_descriptive_track'] = $row->descriptive_track;
    $data['mapped_data_source_url'] = $row->data_source_url;
    $data['program'] = $row->program;
    $data['version'] = $row->version;
    $data['additional_info'] = $row->additional_info;
    $data['methods_citation'] = $row->other_methods;
    $data['filename'] = $row->filename;
    $data['md5sum'] = $row->md5sum;
    $data['sha512'] = $row->sha512;
    $data['created'] = $row->created;
    $data['dataset_type'] = 'Mapped dataset';
    $names = implode(', ', array_keys($data));
    $values = implode("', '", array_values($data));
    db_query("insert into {public.ds_submissions}($names) values('$values')");
  }


  // Select table ds_organism_assembly
  unset($data);
  $query = db_select("ds_organism_assembly", "gp");
  // Select fields
  $query->fields("gp");

  // Execute query
  $result = $query->execute();

  // Looping for filling the table rows  
  while ($row = $result->fetchObject()) {
    $data['uid'] = $row->uid;
    $data['name'] = $row->name;
    $data['email'] = $row->email;
    $data['organism'] = $row->organism;
    $data['description'] = $row->description;
    $data['organism_image_filename'] = $row->organism_image_url;
    $data['is_curate_assembly'] = $row->is_curate_assembly;
    $data['is_same'] = $row->is_same;
    $data['manual_curation_name'] = $row->manual_curation_name;
    $data['manual_curation_email'] = $row->manual_curation_email;
    $data['need_assistance'] = $row->need_assistance;
    $data['reason'] = $row->reason;
    $data['time_from'] = $row->time_from;
    $data['time_to'] = $row->time_to;
    $data['no_timeframe'] = $row->no_timeframe;
    $data['dataset_name'] = $row->assembly_name;
    $data['dataset_version'] = $row->assembly_version;
    $data['methods_citation'] = $row->assembly_method;
    $data['dataset_is_publish'] = $row->is_publish;
    $data['dataset_publish_field_data'] = $row->publish_field_data;
    $data['other_notes'] = $row->other_notes;
    $data['assembly_geo_location'] = $row->geo_location;
    $data['assembly_tissues_located'] = $row->tissues_located;
    $data['assembly_gender'] = $row->gender;
    $data['other_gender'] = $row->other_gender;
    $data['data_source_strain'] = $row->data_source_strain;
    $data['data_source_notes'] = $row->data_source_notes;
    $data['data_source_seqplatform'] = $row->data_source_seqplatform;
    $data['assembly_data_source_url'] = $row->data_source_url;
    $data['filename'] = $row->filename;
    $data['md5sum'] = $row->md5sum;
    $data['sha512'] = $row->sha512;
    $data['created'] = $row->created;
    $data['dataset_type'] = 'Assembly';
    $names = implode(', ', array_keys($data));
    $values = implode("', '", array_values($data));
    db_query("insert into {public.ds_submissions}($names) values('$values')");
  }

  return $output;
}


/**
 *  Admin interface to approve/reject the organisms add by the
 * co-ordinators. This is the "Request project list" tab.
 * located at: admin/structure/datasets
 *
 * @return string
 */
function tripal_chado_datasets_dataset_coordinator_organisms() {
  $output = "";

  if (isset($_GET['sort']) && isset($_GET['order'])) {
    // Sort it Ascending or Descending?
    if ($_GET['sort'] == 'asc') {
      $sort = 'ASC';
    }
    else {
      $sort = 'DESC';
    }

    // Which column will be sorted
    switch ($_GET['order']) {
      case 'Request User':
        $order = 'fullname';
        break;
      case 'Email':
        $order = 'email';
        break;
      case 'Scientific Name':
        $order = 'genus';
        break;
      case 'Status':
        $order = 'status';
        break;
      default:
        $order = 'created';
    }
  }
  else {
    // Default sort
    $sort = 'DESC';
    $order = 'created';
  }
  $query = db_select("ds_request_project", "rp");
  $query->fields("rp", [
    "id",
    "fullname",
    "email",
    "genus",
    "species",
    "is_ncbi_submitted",
    "involved_in_generation",
    "status",
    "created",
  ]);
  $query->orderBy($order, $sort);
  $query = $query->extend('TableSort')->extend('PagerDefault')->limit(25);
  $result = $query->execute();

  $header = [
    ["data" => t('Request User'), "field" => "fullname"],
    ["data" => t('Email'), "field" => "email"],
    ["data" => t('Scientific Name'), "field" => "scientific_name"],
    ["data" => t('Submitted GA to NCBI?'), "field" => "genome_assembly_hosted"],
    [
      "data" => t('Involved in the <br>generation of GA?'),
      "field" => "involved_in_generation",
    ],
    ["data" => t('Status'), "field" => "status"],
    ["data" => t('Created'), "field" => "created", 'sort' => 'desc'],
    [],
  ];
  $rows = [];
  $i = 0;

  // Looping for filling the table rows
  while ($data = $result->fetchObject()) {
    $rows[$i][] = $data->fullname;
    $rows[$i][] = $data->email;
    $rows[$i][] = $data->genus . " " . $data->species;
    $rows[$i][] = $data->is_ncbi_submitted;
    $rows[$i][] = $data->involved_in_generation;
    $status = '';
    if ($data->status == 1) {
      $status = 'Approved';
    }
    elseif ($data->status == 0) {
      $status = 'Rejected';
    }
    elseif ($data->status == 2) {
      $status = 'Pending';
    }
    $rows[$i][] = $status;
    $rows[$i][] = date('M d Y h:i:s A', $data->created);
    $rows[$i][] = l(t('Edit'), 'datasets/organism/' . $data->id . '/edit');
    $i++;
  }
  // Output of table with the paging
  $output = theme_table(
      [
        "header" => $header,
        "rows" => $rows,
        "attributes" => [],
        "sticky" => TRUE,
        // Table header will be sticky
        "caption" => "",
        "colgroups" => [],
        "empty" => t("Table has no row!")
        // The message to be displayed if table is empty
      ]
    ) . theme("pager");

  return $output;
}

