<?php

/**
 * submit a dataset information form
 */

function tripal_chado_datasets_request_form($form, &$form_state) {

  //i5k specific code.
  $link = $GLOBALS['base_url'] . '/long-term-i5k-workspace-project-management';
  if (drupal_valid_path($link)) {
    $form['information_markup'] = [
      '#markup' => t('<div id="text">Please note that this dataset will be visible to the public in the JBrowse genome browser. Contact us if this dataset needs to remain private. Refer to our <a href="' . $GLOBALS['base_url'] . '/data-management-policy">data management</a> and <a href="' . $GLOBALS['base_url'] . '/long-term-i5k-workspace-project-management">long-term management policy</a> documents for information about the data types that we store and our long-term data management policy.</div><br>'),
    ];
  }

  $form['data_provider'] = [
    '#type' => 'fieldset',
    '#title' => t('Submitter information'),
    '#attributes' => ['class' => ['assembly']],
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#prefix' => '<div class="col-lg-12">',
    '#suffix' => '</div>',
  ];
  global $user;

  $form['data_provider']['name'] = [
    '#type' => 'textfield',
    '#title' => t('Submitter Name'),
    '#maxlength' => 255,
    '#required' => TRUE,
    '#value' => $user->name,
    '#disabled' => TRUE,
  ];

  $form['data_provider']['email'] = [
    '#type' => 'textfield',
    '#title' => t('Email Address'),
    '#maxlength' => 255,
    '#required' => TRUE,
    '#value' => $user->mail,
    '#disabled' => TRUE,
  ];


  $form['general_information'] = [
    '#type' => 'fieldset',
    '#title' => t('General dataset information'),
    '#attributes' => ['class' => ['assembly']],
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#prefix' => '<div class="col-lg-12">',
    '#suffix' => '</div>',
  ];


  $organism_options = chado_get_organism_select_options(FALSE);

  $form['general_information']['organism'] = [
    '#type' => 'select',
    '#title' => t('Organism'),
    '#options' => $organism_options,
    '#multiple' => FALSE,
    '#required' => TRUE,
    '#prefix' => '<div class="col-lg-6">',
    '#suffix' => '</div>',
    '#default_value' => 0
  ];

  $form['general_information']['dataset_name'] = [
    '#type' => 'textfield',
    '#title' => t('Dataset name'),
    '#description' => t('Name of the dataset that is used in the genome browser and in the community.'),
    '#maxlength' => 255,
    '#required' => TRUE,
    '#prefix' => '<div class="col-lg-6">',
    '#suffix' => '</div>',
  ];

  $form['general_information']['program'] = [
    '#type' => 'textfield',
    '#title' => t('Program'),
    '#description' => t('Name of program(s) used to generate dataset.'),
    '#required' => TRUE,
    '#prefix' => '<div class="col-lg-6">',
    '#suffix' => '</div>',
  ];

  $form['general_information']['dataset_version'] = [
    '#type' => 'textfield',
    '#title' => t('Dataset Version'),
    '#maxlength' => 255,
    '#required' => TRUE,
    '#prefix' => '<div class="col-lg-6">',
    '#suffix' => '</div>',
  ];

  $form['general_information']['version'] = [
    '#type' => 'textfield',
    '#title' => t('Program version'),
    '#required' => TRUE,
    '#prefix' => '<div class="col-lg-6">',
    '#suffix' => '</div>',
  ];

  $is_download_options = ['Yes' => 'Yes', 'No' => 'No'];
  $form['general_information']['is_download'] = [
    '#type' => 'select',
    '#title' => t('Should we make this file available for download in our <a href="/content/data-downloads" target="_BLANK">Data Downloads</a> section?'),
    '#options' => $is_download_options,
    '#limit_validation_errors' => [],
    '#required' => TRUE,
    '#prefix' => '<div class="col-lg-6">',
    '#suffix' => '</div>',
  ];

  $form['general_information']['additional_info'] = [
    '#type' => 'textfield',
    '#title' => t('Additional Information'),
    '#description' => t('Special settings or parameters used.'),
    '#required' => FALSE,
    '#prefix' => '<div class="col-lg-6">',
    '#suffix' => '</div>',
  ];

  $dataset_is_publish_options = [
    '' => 'Select',
    'Yes' => 'Yes',
    'No' => 'No',
  ];
  $dataset_is_publish_selected = !empty($form_state['values']['dataset_is_publish']) ? $form_state['values']['dataset_is_publish'] : '';

  $form['general_information']['dataset_is_publish'] = [
    '#type' => 'select',
    '#options' => $dataset_is_publish_options,
    '#title' => t('Is the dataset published?'),
    '#default_value' => $dataset_is_publish_selected,
    '#ajax' => [
      'callback' => 'tripal_chado_datasets_is_publish_dropdown_callback',
      'wrapper' => 'fieldset_dataset_is_publish',
      'method' => 'replace',
      'effect' => 'fade',
    ],
    '#limit_validation_errors' => [],
    '#required' => TRUE,
    '#prefix' => '<div class="col-lg-6">',
    '#suffix' => '</div>',
  ];

  $form['general_information']['methods_citation'] = [
    '#type' => 'textfield',
    '#title' => t('Methods Citation (DOI)'),
    '#size' => 22,
    '#description' => t('Enter the DOI of the relevant Methods Citation.'),
    '#required' => FALSE,
    '#prefix' => '<div class="col-lg-6">',
    '#suffix' => '</div>',
  ];

  $form['general_information']['fieldset_dataset_is_publish'] = [
    '#prefix' => '<div id="fieldset_dataset_is_publish">',
    '#suffix' => '</div>',
    '#type' => 'fieldset',
  ];

  // Since dropdown return TRUE or FALSE, we have to check that
  // $form_state has been filled as well as what it contains.
  if (!empty($form_state['input']['dataset_is_publish']) && isset($form_state['input']['dataset_is_publish'])) {
    $description = ($form_state['input']['dataset_is_publish'] == 'Yes') ? t('please provide DOI citation') : t('please indicate whether <a href="http://www.ncbi.nlm.nih.gov/pmc/articles/PMC3073843/?tool=pubmed" target="_BLANK">Toronto</a>/<a href="https://www.genome.gov/pages/research/wellcomereport0303.pdf" target="_BLANK">Ft. Lauderdale</a> conditions of data re-use should apply');
    $form['general_information']['fieldset_dataset_is_publish']['dataset_publish_field_data'] = [
      '#type' => 'textfield',
      '#title' => t('If ') . $form_state['input']['dataset_is_publish'] . ', ' . $description,
    ];
  }


  $form['information_markup1'] = [
    '#markup' => t('<div id="text"><h3>Select the dataset type that best matches your submission and fill out additional information:</h3></div><br>'),
  ];

  /************************** Genome assembly information *****************************/
  $form['assembly'] = [
    '#type' => 'fieldset',
    '#title' => t('Genome assembly information'),
    '#attributes' => ['class' => ['assembly']],
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#prefix' => '<div class="col-lg-12">',
    '#suffix' => '</div>',
  ];

  // Project Background
  $form['assembly']['project'] = [
    '#type' => 'fieldset',
    '#title' => t('Project Background'),
    '#attributes' => ['class' => ['aa_information']],
    '#collapsible' => TRUE, // Added
    '#collapsed' => FALSE,  // Added
    '#prefix' => '<div class="col-lg-12">',
    '#suffix' => '</div>',
  ];

  $form['assembly']['project']['description'] = [
    '#type' => 'textarea',
    '#title' => t('Project description to display in your organism page'),
    '#required' => FALSE,
  ];

  $curate_options = ['' => '-Select-', 'Yes' => 'Yes', 'No' => 'No'];
  $form['assembly']['project']['is_curate_assembly'] = [
    '#type' => 'select',
    '#title' => t('Will you manually curate this assembly using our tools?'),
    '#options' => $curate_options,
    '#ajax' => [
      'callback' => 'tripal_chado_manual_curation_dropdown_callback',
      'wrapper' => 'manual_curation',
    ],
    '#limit_validation_errors' => [],
    '#required' => FALSE,
  ];
  $form['assembly']['project']['manual_curation'] = [
    '#type' => 'fieldset',
    '#title' => t(''),
    '#attributes' => ['class' => ['manual_curation']],
    '#prefix' => '<div id="manual_curation">',
    '#suffix' => '</div>',
    '#collapsible' => TRUE, // Added
    '#collapsed' => FALSE,  // Added
  ];
  if (!empty($form_state['input']['is_curate_assembly']) && ($form_state['input']['is_curate_assembly'] == 'Yes')) {
    $form['assembly']['project']['manual_curation']['is_same'] = [
      '#type' => 'checkbox',
      '#title' => t('Is the curation coordinator the same as the genome coordinator?'),
      '#default_value' => FALSE,
      '#ajax' => [
        'callback' => 'tripal_chado_name_email_callback',
        'wrapper' => 'name_email',
      ],
    ];
    $form['assembly']['project']['manual_curation']['name_email'] = [
      '#type' => 'fieldset',
      '#title' => t(''),
      '#attributes' => ['class' => ['name_email']],
      '#prefix' => '<div id="name_email">',
      '#suffix' => '</div>',
      '#collapsible' => TRUE, // Added
      '#collapsed' => FALSE,  // Added
    ];
    $cordinator_name = '';
    $name_attributes = [];
    if ($form_state['input']['is_same'] == 1 && !empty($form_state['input']['name'])) {
      $cordinator_name = $form_state['input']['name'];
      $name_attributes = ['readonly' => 'readonly'];
      $form['assembly']['project']['manual_curation']['name_email']['manual_curation_name'] = [
        '#type' => 'textfield',
        '#title' => t('Co-ordinator name'),
        '#required' => FALSE,
        '#attributes' => $name_attributes,
        '#value' => $cordinator_name,
      ];
    }
    else {
      $form['assembly']['project']['manual_curation']['name_email']['manual_curation_name'] = [
        '#type' => 'textfield',
        '#title' => t('Co-ordinator name'),
        '#required' => FALSE,
      ];
    }
    $cordinator_email = '';
    $email_attributes = [];
    if ($form_state['input']['is_same'] == 1 && !empty($form_state['input']['email'])) {
      $cordinator_email = $form_state['input']['email'];
      $email_attributes = ['readonly' => 'readonly'];
      $form['assembly']['project']['manual_curation']['name_email']['manual_curation_email'] = [
        '#type' => 'textfield',
        '#title' => t('Co-ordinator email'),
        '#required' => FALSE,
        '#attributes' => $email_attributes,
        '#value' => $cordinator_email,
      ];
    }
    else {
      $form['assembly']['project']['manual_curation']['name_email']['manual_curation_email'] = [
        '#type' => 'textfield',
        '#title' => t('Co-ordinator email'),
        '#required' => FALSE,
      ];
    }
    $assistance_options = ['Yes' => 'Yes', 'No' => 'No'];
    $form['assembly']['project']['manual_curation']['need_assistance'] = [
      '#type' => 'select',
      '#title' => t('Do you need assistance developing an Official Gene Set?'),
      '#options' => $assistance_options,
      '#ajax' => [
        'callback' => 'tripal_chado_assistance_dropdown_callback',
        'wrapper' => 'assistance',
      ],
      '#limit_validation_errors' => [],
      '#required' => FALSE,
    ];
    $form['assembly']['project']['manual_curation']['assistance'] = [
      '#type' => 'fieldset',
      '#prefix' => '<div id="assistance">',
      '#suffix' => '</div>',
    ];
    if (isset($form_state['input']['need_assistance']) && ($form_state['input']['need_assistance'] == 'Yes')) {
      $form['assembly']['project']['manual_curation']['assistance']['reason'] = [
        '#type' => 'textfield',
        '#title' => t('Reason'),
        '#required' => TRUE,
      ];
    }

    $form['assembly']['project']['manual_curation']['tf_condition_text'] = [
      '#type' => 'fieldset',
      '#title' => t('Specify curation time frame <div id="tf_required">*</div>'),
      '#prefix' => '<div id="tf_condition_text">',
      '#suffix' => '</div>',
      '#attributes' => ['class' => ['tf_or_condition']],
      '#collapsible' => TRUE, // Added
      '#collapsed' => FALSE,  // Added
    ];

    $form['assembly']['project']['manual_curation']['tf_condition_text']['time_from'] = [
      '#type' => 'date_popup',
      '#date_format' => 'm/d/Y',
      '#title' => t(''),
      '#attributes' => ['placeholder' => t('Start date')],
      '#ajax' => [
        'callback' => 'datepopup_callback',
        'wrapper' => 'tf_condition',
      ],
    ];

    $form['assembly']['project']['manual_curation']['tf_condition_text']['time_to'] = [
      '#type' => 'date_popup',
      '#date_format' => 'm/d/Y',
      '#attributes' => ['placeholder' => t('End date')],
      '#title' => t(''),
      '#required' => FALSE,
      '#ajax' => [
        'callback' => 'datepopup_callback',
        'wrapper' => 'tf_condition',
      ],
    ];

    $form['assembly']['project']['manual_curation']['tf_condition1'] = [
      '#markup' => t('<div id="or_css">(OR)</div>'),
    ];

    $form['assembly']['project']['manual_curation']['tf_condition'] = [
      '#type' => 'fieldset',
      '#title' => t(''),
      '#prefix' => '<div id="tf_condition">',
      '#suffix' => '</div>',
      '#collapsible' => TRUE, // Added
      '#collapsed' => FALSE,  // Added
    ];
    $form['assembly']['project']['manual_curation']['tf_condition']['no_timeframe'] = [
      '#type' => 'checkbox',
      '#title' => t('There is no set time frame for curation.'),
      '#default_value' => 0,
      '#ajax' => [
        'callback' => 'no_timeframe_callback',
        'wrapper' => 'tf_condition_text',
      ],
      '#limit_validation_errors' => [],
    ];
    if (!empty($form_state['input']['time_to']['date']) || !empty($form_state['input']['time_from']['date'])) {
      $form['assembly']['project']['manual_curation']['tf_condition']['no_timeframe']['#attributes']['disabled'] = 'disabled';
    }
  }


  /*Data Source Information*/
  $form['assembly']['data_source'] = [
    '#type' => 'fieldset',
    '#title' => t('Data source information'),
    '#attributes' => ['class' => ['assembly_data_source']],
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#prefix' => '<div class="col-lg-12">',
    '#suffix' => '</div>',
  ];

  $form['assembly']['data_source']['assembly_geo_location'] = [
    '#type' => 'textfield',
    '#title' => t('Geographic location (latitude and longitude)'),
    '#required' => FALSE,
    '#description' => "The geographical origin of the sample as defined by latitude and longitude. The values should be reported in decimal degrees and in WGS84 system",
  ];

  $form['assembly']['data_source']['assembly_tissues_located'] = [
    '#type' => 'textfield',
    '#title' => t('Tissues/Life stage included'),
    '#required' => FALSE,
    '#description' => "(Whole individual/ antenna / pooled larva / pooled adult female/ etc.)",
  ];

  $assembly_gender_options = [
    '' => 'Select',
    'Female' => 'Female',
    'Male' => 'Male',
    'Pooled' => 'Pooled',
    'NA' => 'Other',
  ];
  $assembly_gender_selected = !empty($form_state['values']['assembly']['data_source']['assembly_gender']) ? $form_state['values']['assembly']['data_source']['assembly_gender'] : '';
  $form['assembly']['data_source']['assembly_gender'] = [
    '#type' => 'select',
    '#options' => $assembly_gender_options,
    '#title' => t('Sex'),
    '#default_value' => $assembly_gender_selected,
    '#ajax' => [
      'callback' => 'tripal_chado_datasets_assembly_datasource_gender_dropdown_callback',
      'wrapper' => 'fieldset_assembly_gender_textfield',
      'method' => 'replace',
      'effect' => 'fade',
    ],
    '#limit_validation_errors' => [],
    '#required' => FALSE,
  ];

  $form['assembly']['data_source']['fieldset_assembly_gender_textfield'] = [
    '#prefix' => '<div id="fieldset_assembly_gender_textfield">',
    '#suffix' => '</div>',
    '#type' => 'fieldset',
  ];

  if (!empty($form_state['input']['assembly_gender']) && ($form_state['input']['assembly_gender'] == 'NA')) {
    $form['assembly']['data_source']['fieldset_assembly_gender_textfield']['other_gender'] = [
      '#type' => 'textfield',
      '#title' => t('Other (Sex)'),
    ];
  }

  $form['assembly']['data_source']['data_source_strain'] = [
    '#type' => 'textfield',
    '#title' => t('Strain'),
  ];

  $form['assembly']['data_source']['data_source_notes'] = [
    '#type' => 'textfield',
    '#title' => t('Other notes'),
  ];

  $form['assembly']['data_source']['data_source_seqplatform'] = [
    '#type' => 'textfield',
    '#title' => t('Sequencing method'),
    '#description' => "Sequencing method used; e.g. Sanger, pyrosequencing, ABI-solid",
    '#required' => FALSE,
  ];

  // $form['assembly']['data_source']['assembly_data_source_url'] = array(
  //   '#type' => 'textfield',
  //   '#title' => t('Data source URL'),
  //   '#required' => FALSE,
  // );

  $form['assembly']['info'] = [
    '#type' => 'fieldset',
    '#title' => t('Assembly information'),
    '#attributes' => ['class' => ['aa_information']],
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#prefix' => '<div class="col-lg-12">',
    '#suffix' => '</div>',
  ];

  $form['assembly']['info']['assembly_accession'] = [
    '#type' => 'textfield',
    '#title' => t('NCBI/INSDC Genome Assembly accession #'),
    '#maxlength' => 255,
    '#required' => FALSE,
  ];

  $form['assembly']['info']['additional_other_notes'] = [
    '#type' => 'textfield',
    '#title' => t('Other notes'),
    '#description' => "Include additional notes if desired",
  ];


  /***************************************************** End of Genome assembly information ********************************************************/

  /***************************** Gene set information -  To make the fieldset collapsible ******************************/
  $form['gene_set'] = [
    '#type' => 'fieldset',
    '#title' => t('Gene set information'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#prefix' => '<div class="col-lg-12">',
    '#suffix' => '</div>',
  ];

  $form['gene_set']['gene_set_descriptive_track'] = [
    '#type' => 'textfield',
    '#title' => t('Descriptive track name for JBrowse and Apollo'),
    '#description' => t('This is how you and others will identify the track in Jbrowse and Apollo - short yet descriptive is best'),
    '#required' => FALSE,
  ];

  $ogs_options = ['' => '-Select-', 'Yes' => 'Yes', 'No' => 'No'];
  $form['gene_set']['is_ogs'] = [
    '#type' => 'select',
    '#title' => t('Is this an Official Gene Set?'),
    '#description' => t('We will generate gene pages for Official Gene Sets - specify yes if the gene set is viewed as the community standard for this genome assembly'),
    '#options' => $ogs_options,
    '#limit_validation_errors' => [],
    '#required' => FALSE,
  ];

  // $gene_set_publish_options = Array(
  //   '' => '-Select-',
  //   'Yes' => 'Yes',
  //   'No' => 'No'
  // );
  // $gene_set_selected = isset($form_state['values']['gene_set_is_publish']) ? $form_state['values']['gene_set_is_publish'] : key($gene_set_publish_options);

  // $form['gene_set']['gene_set_is_publish'] = array(
  //   '#type' => 'select',
  //   '#options' => $gene_set_publish_options,
  //   '#title' => t('Are the data published?'),
  //   '#default_value' => $gene_set_selected,
  //   '#ajax' => array(
  //     'callback' => 'gene_set_is_publish_dropdown_callback',
  //     'wrapper' => 'fieldset_gene_set_is_publish',
  //     'method' => 'replace',
  //     'effect' => 'fade',
  //   ),
  //   '#required' => FALSE
  // );

  // $form['gene_set']['fieldset_gene_set_is_publish'] = array(
  //   '#prefix' => '<div id="fieldset_gene_set_is_publish">',
  //   '#suffix' => '</div>',
  //   '#type' => 'fieldset',
  // );

  // Since dropdown return TRUE or FALSE, we have to check that
  // $form_state has been filled as well as what it contains.
  // if (!empty($form_state['input']['gene_set_is_publish']) && isset($form_state['input']['gene_set_is_publish'])) {
  //   $description = ($form_state['input']['gene_set_is_publish'] == 'Yes')? t('please provide citation'):t('please indicate whether <a href="http://www.ncbi.nlm.nih.gov/pmc/articles/PMC3073843/?tool=pubmed" target="_BLANK">Toronto</a>/<a href="http://www.wellcome.ac.uk/stellent/groups/corporatesite/@policy_communications/documents/web_document/wtd003207.pdf" target="_BLANK">Ft. Lauderdale</a> conditions of data re-use should apply');
  //   $publish_title = 'gene_set_publish_field_data';
  //   $form['gene_set']['fieldset_gene_set_is_publish'][$publish_title] = array(
  //     '#type' => 'textfield',
  //     '#title' => t('If ') . $form_state['input']['gene_set_is_publish'] . ', ' . $description,
  //   );
  // }


  /********************* End of Gene set information -  To make the fieldset collapsible **********************/

  /**************************************** SUBMIT MAPPED DATASET ***************************************/

  // To make the fieldset collapsible
  $form['mapped_dataset'] = [
    '#type' => 'fieldset',
    '#title' => t('Mapped dataset'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#prefix' => '<div class="col-lg-12">',
    '#suffix' => '</div>',
  ];

  $form['mapped_dataset']['mapped_dataset_geo_location'] = [
    '#type' => 'textfield',
    '#title' => t('Geographic location (latitude and longitude)'),
    '#required' => FALSE,
    '#description' => "The geographical origin of the sample as defined by latitude and longitude. The values should be reported in decimal degrees and in WGS84 system",
  ];

  $form['mapped_dataset']['mapped_dataset_tissues_located'] = [
    '#type' => 'textfield',
    '#title' => t('Tissues/Life stage included'),
    '#required' => FALSE,
    '#description' => "(Whole individual/ antenna / pooled larva / pooled adult female/ etc.)",
  ];

  $mapped_gender_options = [
    '' => 'Select',
    'Female' => 'Female',
    'Male' => 'Male',
    'Pooled' => 'Pooled',
    'NA' => 'Other/NA',
  ];
  $mapped_gender_selected = isset($form_state['values']['mapped_data_gender']) ? $form_state['values']['mapped_data_gender'] : key($mapped_gender_options);
  $form['mapped_dataset']['mapped_gender'] = [
    '#type' => 'select',
    '#options' => $mapped_gender_options,
    '#title' => t('Sex'),
    '#default_value' => $mapped_gender_selected,
    '#ajax' => [
      'callback' => 'tripal_chado_datasets_gender_dropdown_callback',
      'wrapper' => 'fieldset_mapped_gender_textfield',
      'method' => 'replace',
      'effect' => 'fade',
    ],
    '#limit_validation_errors' => [],
    '#required' => FALSE,
  ];

  $form['mapped_dataset']['fieldset_mapped_gender_textfield'] = [
    '#prefix' => '<div id="fieldset_mapped_gender_textfield">',
    '#suffix' => '</div>',
    '#type' => 'fieldset',
  ];

  if (isset($form_state['input']['mapped_gender']) && ($form_state['input']['mapped_gender'] == 'NA')) {
    $form['mapped_dataset']['fieldset_mapped_gender_textfield']['mapped_other_gender'] = [
      '#type' => 'textfield',
      '#title' => t('Other/NA (Sex)'),
    ];
  }

  $form['mapped_dataset']['sequence_platform'] = [
    '#type' => 'textfield',
    '#title' => t('Sequencing method'),
    '#required' => FALSE,
    '#description' => "(Sequencing method used; e.g. Sanger, pyrosequencing, ABI-solid)",
  ];

  $form['mapped_dataset']['mapped_dataset_descriptive_track'] = [
    '#type' => 'textfield',
    '#title' => t('Descriptive track name for JBrowse and Apollo'),
    '#required' => FALSE,
    '#description' => t('This is how you and others will identify the track in Jbrowse and Apollo - short yet descriptive is best'),
    '#description' => "(this is what the users will see in Web Apollo, so short yet explanatory is best)",
  ];

  $form['mapped_dataset']['mapped_data_source_url'] = [
    '#type' => 'textfield',
    '#title' => t('NCBI SRA accession number(s)'),
    '#required' => FALSE,
  ];

  /************************* END OF MAPPED DATASET *************************************/

  $form['information_markup2'] = [
    '#markup' => t('<div id="text"><h3>Select one of the following upload options:</h3></div><br>'),
  ];

  //FILE INFORMATION
  $form['fileinfo'] = [
    '#type' => 'fieldset',
    '#title' => t('File information'),
    '#attributes' => ['class' => ['assembly']],
    '#collapsible' => TRUE, // Added
    '#collapsed' => FALSE,  // Added
    '#prefix' => '<div class="col-lg-12">',
    '#suffix' => '</div>',
  ];

  //Conditionally create a link for this section title.  Only do so if the site has this page.
  $title = 'SHA512';
  $link = '/how-generate-sha-512-checksum';
  if (drupal_valid_path($link)){
    $title = l(t('SHA512'), '/how-generate-sha-512-checksum', ['attributes' => ['target' => '_blank']]);
  }

  $form['fileinfo']['SHA512'] = [
    '#type' => 'textfield',
    '#title' => $title,
    '#description' => t('SHA512.'),
    '#required' => FALSE,
    '#prefix' => t('You can transfer your file to us in one of three ways: upload your file from your computer, give us a URL to the file, or transmit via our ftp site. Please select only one option, and enter the sha512 checksum for the file.<br /><br />'),
  ];

  $form['fileinfo']['uffu'] = [
    '#type' => 'fieldset',
    '#title' => t('<b>Upload file from URL</b>'),
    '#attributes' => ['class' => ['assembly']],
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  ];

  $form['fileinfo']['uffu']['fileurl'] = [
    '#type' => 'textfield',
    '#size' => 200,
    '#maxlength' => 200,
    "#upload_validators" => [
      "file_validate_extensions" => ["gz zip gff gff3 txt fa fasta fna faa bam bai bigwig bw tar bz2 gtf"],
      "file_validate_size" => [1024 * 1024 * 1024 * 5], // 5 GB
    ],
  ];

  $form['fileinfo']['markup_text1'] = [
    '#prefix' => '<div>',
    '#markup' => t('OR'),
    '#suffix' => '</div>',
  ];

  $form['fileinfo']['uffc'] = [
    '#type' => 'fieldset',
    '#title' => t('<b>Upload file from Computer</b>'),
    '#attributes' => ['class' => ['assembly']],
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  ];

  $form['fileinfo']['uffc']['upload'] = [
    '#type' => 'managed_file',
    '#description' => t('Please ensure your file meets the following requirements.'),
    '#upload_location' => 'public://',
    '#progress_indicator' => 'bar',
    '#autostart' => TRUE,
    "#upload_validators" => [
      "file_validate_extensions" => ["gz zip gff gff3 txt fa fasta fna faa bam bai bigwig bw tar bz2 gtf"],
      "file_validate_size" => [1024 * 1024 * 1024 * 5], // 5GB
    ],
    '#file_resup_max_files' => 1,
    '#field_name' => 'upload',
    '#field_parents' => [],
    '#language' => LANGUAGE_NONE,
    '#required' => FALSE,
  ];
  $form['fileinfo']['uffc']['upload']['#file_resup_upload_validators'] = $form['fileinfo']['uffc']['upload']['#upload_validators'];
  $form['fileinfo']['uffc']['upload']['#process'] = [
    'file_managed_file_process',
    'file_resup_field_widget_process',
  ];
  $form['fileinfo']['uffc']['upload']['#file_value_callbacks'][] = 'file_resup_field_widget_value';

  $form['fileinfo']['uffc']['markup_text'] = [
    '#prefix' => '<div id="files_markup">',
    '#markup' => t('Maximum recommended file size: 5 GB. If multiple files need to be uploaded, please compress into a single archive. <br> Allowed file extensions: gz zip gff gff3 txt fa fasta fna faa bam bai bigwig bw tar bz2 gtf.<br>'),
    '#suffix' => '</div>',
  ];

  $form['fileinfo']['markup_text2'] = [
    '#prefix' => '<div>',
    '#markup' => t('OR'),
    '#suffix' => '</div>',
  ];

  $form['fileinfo']['ufff'] = [
    '#type' => 'fieldset',
    '#title' => t('<b>Upload file via FTP</b>'),
    '#attributes' => ['class' => ['assembly']],
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  ];

  $form['fileinfo']['ufff']['fileftp'] = [
    '#type' => 'textfield',
    '#size' => 200,
    '#maxlength' => 200,
    "#upload_validators" => [
      "file_validate_extensions" => ["gz zip gff gff3 txt fa fasta fna faa bam bai bigwig bw tar bz2 gtf"],
      "file_validate_size" => [1024 * 1024 * 1024 * 5], // 5 GB
    ],
    '#suffix' => t('Enter the file name here. We will email you instructions on how to upload the file via ftp.'),
  ];

  // Enabling honeypot to avoid spam
  honeypot_add_form_protection($form, $form_state, [
    'honeypot',
    'time_restriction',
  ]);

  $form['submit'] = [
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#prefix' => '<div class="col-lg-6">',
    '#suffix' => '</div>',
  ];


  return $form;
}


/**
 * Submit dataset form validation
 *
 * Data provider name, affiliation should enter only letters @ - _.
 * Email validation
 * Data source url validation
 *
 */
function tripal_chado_datasets_request_form_validate($form, &$form_state) {
  // dsm($form_state);
  $values = $form_state['values'];
  $name = $values['name'];

  // no file select
  $isUpload = FALSE;
  if (!empty($values['fileurl'])) {
    $isUpload = TRUE;
  }
  elseif ($values['upload']) {
    $isUpload = TRUE;
  }
  elseif (!empty($values['fileftp'])) {
    $isUpload = TRUE;
  }
  if (!$isUpload) {
    $form_state['complete form']['fileinfo']['uffu']['#collapsed'] = FALSE;
    form_set_error('fileurl', t('Atleast one file must be entered.'));
  }

  // more than one file select
  if ($values['upload'] && valid_url($values['fileurl'], 'TRUE')) {
    $form_state['complete form']['fileinfo']['uffu']['#collapsed'] = FALSE;
    form_set_error('fileurl', t('Only one file upload type can be choosen.'));
  }
  elseif ($values['upload'] && !empty($values['fileftp'])) {
    $form_state['complete form']['fileinfo']['ufff']['#collapsed'] = FALSE;
    form_set_error('fileftp', t('Only one file upload type can be choosen.'));
  }
  elseif (!empty($values['fileftp']) && valid_url($values['fileurl'], 'TRUE')) {
    $form_state['complete form']['fileinfo']['uffu']['#collapsed'] = FALSE;
    form_set_error('fileurl', t('Only one file upload type can be choosen.'));
  }

  // file url must be valid
  if (!empty($values['fileurl']) && !valid_url($values['fileurl'], 'TRUE')) {
    $form_state['complete form']['fileinfo']['uffu']['#collapsed'] = FALSE;
    form_set_error('fileurl', t('You must enter a valid url.'));
  }
  /*elseif (!empty($values['fileurl'])) {
  $file_validate_extensions = $form_state['complete form']['fileurl']['#upload_validators']['file_validate_extensions'][0];
  $fileurl_data = explode('.', $values['fileurl']);
  if ( strpos($file_validate_extensions, end($fileurl_data)) === false ) {
  form_set_error('fileurl', t('File url cannot be uploaded. Invalid file extensions.'));
  }
  }*/

  // if (!preg_match('!^[\w @-]*$!', $name)) {
  //   form_set_error('name', t('Your Name can only contains numbers, letters @  - _.'));
  // }
  // if (isset($values['email']) && !valid_email_address($values['email'])) {
  //   form_set_error('email', t('You must enter a valid e-mail address.'));
  // }
  // $affiliation = $values['affiliation'];
  // if (!preg_match('!^[\w @-]*$!', $affiliation)) {
  //   form_set_error('affiliation', t('Your
  //   Affiliation can only contains numbers, letters @  - _.'));
  // }

  if (!empty($values['dataset_is_publish']) && empty($values['dataset_publish_field_data'])) {
    $description = ($values['dataset_is_publish'] == 'Yes') ? t('please provide citation') : t('please indicate whether <a href="http://www.ncbi.nlm.nih.gov/pmc/articles/PMC3073843/?tool=pubmed" target="_BLANK">Toronto</a>/<a href="http://www.wellcome.ac.uk/stellent/groups/corporatesite/@policy_communications/documents/web_document/wtd003207.pdf" target="_BLANK">Ft. Lauderdale</a> conditions of data re-use should apply');
    form_set_error('publish_field_data', t('The field \'If ' . $values['dataset_is_publish']) . ' ' . $description . '\' is required');
  }

  // if (!empty($values['data_source_url']) && !valid_url($values['data_source_url'], 'TRUE')) {
  //   form_set_error('data_source_url', t('You must enter a valid url.'));
  // }

  // if (!preg_match('!^[\w -]*$!', $values['methods_citation'])) {
  //   form_set_error('methods_citation', t('Your other methods field can only contains numbers, letters -_.'));
  // }

  $assembly_fields = [
    'assembly_geo_location',
    'assembly_tissues_located',
    'assembly_gender',
    'data_source_strain',
    'data_source_notes',
    'data_source_seqplatform',
    'assembly_accession',
    'other_notes',
    'additional_other_notes',
  ];
  $gene_set_fields = ['gene_set_descriptive_track', 'is_ogs'];
  $mapped_dataset_fields = [
    'mapped_dataset_geo_location',
    'mapped_dataset_tissues_located',
    'mapped_dataset_descriptive_track',
    'mapped_gender',
    'sequence_platform',
    'mapped_dataset_data_source_url',
  ];

  $assemblyIsEmpty = TRUE;
  $gene_setIsEmpty = TRUE;
  $mapped_datasetIsEmpty = TRUE;
  foreach ($assembly_fields as $key => $fieldName) {
    if (isset($values[$fieldName]) && !empty($values[$fieldName])) {
      // echo $fieldName . '-' . $values[$fieldName];exit;
      $assemblyIsEmpty = FALSE;
    }
  }
  foreach ($gene_set_fields as $key => $fieldName) {
    if (isset($values[$fieldName]) && !empty($values[$fieldName])) {
      // echo $fieldName . '-' . $values[$fieldName];exit;
      $gene_setIsEmpty = FALSE;
    }
  }
  foreach ($mapped_dataset_fields as $key => $fieldName) {
    if (isset($values[$fieldName]) && !empty($values[$fieldName])) {
      // echo $fieldName . '-' . $values[$fieldName];exit;
      $mapped_datasetIsEmpty = FALSE;
    }
  }
  if (!$assemblyIsEmpty) {
    /* TIME FRAME VALIDATIONS -- validate empty End date */
    if (!empty($values['is_curate_assembly']) && ($values['is_curate_assembly'] == 'Yes') && ($values['no_timeframe'] == 0) && !empty($values['time_from']) && is_null($values['time_to'])) {
      form_set_error('time_to', t('End date field is required.'));
    }

    if (!empty($values['is_curate_assembly']) && ($values['is_curate_assembly'] == 'Yes') && ($values['no_timeframe'] == 0) && !empty($values['time_to']) && is_null($values['time_from'])) {
      form_set_error('time_from', t('Start date field is required.'));
    }

    if (($values['is_curate_assembly'] == 'Yes') && is_null($values['time_to']) && is_null($values['time_from']) && empty($values['no_timeframe'])) {
      form_set_error('time_from', t('Enter either one of the curation time frame.'));
    }

    if (($values['is_curate_assembly'] == 'Yes') && (!empty($values['time_to']) || !empty($values['time_from'])) && !empty($values['no_timeframe'])) {
      form_set_error('time_from', t('Enter either one of the curation time frame but not both.'));
    }
  }
  if ($assemblyIsEmpty && $gene_setIsEmpty && $mapped_datasetIsEmpty) {
    form_set_error('assembly_geo_location', t('Please fill out information in one of the following sections: Genome assembly information, Gene set information, or Mapped dataset information.'));
  }
  else {
    if ((!$assemblyIsEmpty && !$gene_setIsEmpty) || (!$gene_setIsEmpty && !$mapped_datasetIsEmpty) || (!$mapped_datasetIsEmpty && !$assemblyIsEmpty)) {
      form_set_error('assembly_geo_location', t('Please submit only one dataset at a time.'));
    }
  }
}

/**
 * hook_form submit
 */
function tripal_chado_datasets_request_form_submit($form, &$form_state) {
  global $user;
  $values = $form_state['values'];

  $organism_list = chado_get_organism_select_options(FALSE);

  $assembly_fields = [
    'assembly_geo_location',
    'assembly_tissues_located',
    'assembly_gender',
    'data_source_strain',
    'data_source_notes',
    'data_source_seqplatform',
    'assembly_accession',
    'other_notes',
    'additional_other_notes',
  ];
  $gene_set_fields = ['gene_set_descriptive_track', 'is_ogs'];
  $mapped_dataset_fields = [
    'mapped_dataset_geo_location',
    'mapped_dataset_tissues_located',
    'mapped_dataset_descriptive_track',
    'mapped_gender',
    'mapped_other_gender',
    'sequence_platform',
    'mapped_dataset_data_source_url',
  ];

  $assemblyIsEmpty = TRUE;
  $gene_setIsEmpty = TRUE;
  $mapped_datasetIsEmpty = TRUE;
  foreach ($assembly_fields as $key => $fieldName) {
    if (isset($values[$fieldName]) && !empty($values[$fieldName])) {
      $assemblyIsEmpty = FALSE;
    }
  }
  foreach ($gene_set_fields as $key => $fieldName) {
    if (isset($values[$fieldName]) && !empty($values[$fieldName])) {
      $gene_setIsEmpty = FALSE;
    }
  }
  foreach ($mapped_dataset_fields as $key => $fieldName) {
    if (isset($values[$fieldName]) && !empty($values[$fieldName])) {
      $mapped_datasetIsEmpty = FALSE;
    }
  }
  $dataset_type = '';
  if (!$assemblyIsEmpty) {
    $dataset_type = 'Assembly';
  }
  else {
    if (!$gene_setIsEmpty) {
      $dataset_type = 'Gene prediction';
    }
    else {
      if (!$mapped_datasetIsEmpty) {
        $dataset_type = 'Mapped dataset';
      }
    }
  }
  $values['dataset_type'] = $dataset_type;

  $exclude_fields = [
    'i5kurl',
    'submit',
    'form_build_id',
    'form_token',
    'form_id',
    'op',
    'upload',
    'fileurl',
    'fileftp',
    'honeypot_time',
    'affiliation',
  ];
  $names_col = [];
  $values_col = [];
  $created = time();
  $filename = '';


  $values['time_to'] = (empty($values['time_to']) && !isset($values['time_to'])) ? '0000-00-00' : date('Y-m-d', strtotime($values['time_to']));
  $values['time_from'] = (empty($values['time_from']) && !isset($values['time_from'])) ? '0000-00-00' : date('Y-m-d', strtotime($values['time_from']));

  foreach ($values as $key => $value) {
    if (!in_array($key, $exclude_fields)) {
      if (isset($value) && !empty($value)) {
        array_push($names_col, $key);
        array_push($values_col, "'" . check_plain($value) . "'");
      }
    }
  }

  if (isset($values['upload']) && !empty($values['upload'])) {
    $file = file_load($values['upload']);
    $filename = $file->filename;
    $values['filename'] = $filename;

    if (isset($file)) {
      $file->status = FILE_STATUS_PERMANENT;
      $file = file_save($file);
      //returned updated object
    }
  }

  if (isset($values['fileurl']) && !empty($values['fileurl'])) {
    $filename = $values['fileurl'];
  }

  if (isset($values['fileftp']) && !empty($values['fileftp'])) {
    $filename = $values['fileftp'];
  }

  array_push($names_col, 'filename');
  array_push($values_col, "'" . $filename . "'");
  array_push($names_col, 'uid');
  array_push($values_col, $user->uid);
  array_push($names_col, 'created');
  array_push($values_col, $created);

  $names = implode(',', $names_col);
  $data = implode(',', $values_col);
  db_query("insert into {public.ds_submissions}($names) values($data)");
  // E-mail address of the sender: as the form field is a text field.
  $from = FROM_ADDRESS;
  $to_address = $values['email'];
  $values['organism'] = $organism_list[$values['organism']];

  // Send the e-mail to the recipients using the site default language.
  $sent = drupal_mail('tripal_chado_datasets', 'submit_dataset_email', $to_address, language_default(), $values, $from, TRUE);
  if (empty($sent['result']) || ($sent['result'] != 1)) {
    // handle send fail, $sent ===false when mail fails, but it won't always recognize a failure
    drupal_set_message(t('Failed to send e-mail.'));
  }
  else {
    drupal_set_message(t('Your information is successfully submitted. For your reference we are sending out an email with the fields filled.'));
  }

}


function tripal_chado_datasets_is_publish_dropdown_callback($form, $form_state) {
  return $form['general_information']['fieldset_dataset_is_publish'];
}

function tripal_chado_datasets_assembly_datasource_gender_dropdown_callback($form, $form_state) {
  return $form['assembly']['data_source']['fieldset_assembly_gender_textfield'];
}

function tripal_chado_datasets_gender_dropdown_callback($form, $form_state) {
  return $form['mapped_dataset']['fieldset_mapped_gender_textfield'];
}

function tripal_chado_manual_curation_dropdown_callback($form, $form_state) {
  return $form['assembly']['project']['manual_curation'];
}

function tripal_chado_name_email_callback($form, $form_state) {
  return $form['assembly']['project']['manual_curation']['name_email'];
}

function tripal_chado_assistance_dropdown_callback($form, $form_state) {
  return $form['assembly']['project']['manual_curation']['assistance'];
}
