<?php

/**
 * hook_form
 * - Creates add organism form
 * - Save the data in public schema cordinator_organism table
 */
function request_project($form, &$form_state) {

  $form['#attached']['css'] = [
    drupal_get_path('module', 'datasets') . '/css/request_project.css',
  ];
  $form['header_text'] = [
    '#markup' => '<div id="text">Thank you for your interest in submitting your genome project to the i5k Workspace! Please answer the following questions to help us decide if the resources at the i5k Workspace are a good fit for your project. Refer to our <a href="' . $GLOBALS['base_url'] . '/data-management-policy">data management</a> and <a href="' . $GLOBALS['base_url'] . '/long-term-i5k-workspace-project-management">long-term management policy</a> documents for information about the data types that we store and our long-term data management policy.</div>',
  ];

  $form['genus'] = [
    '#type' => 'textfield',
    '#title' => t('Genus'),
    '#required' => TRUE,
  ];

  $form['species'] = [
    '#type' => 'textfield',
    '#title' => t('Species'),
    '#required' => TRUE,
  ];

  $form['ncbi_taxid'] = [
    '#type' => 'textfield',
    '#title' => t('NCBI Taxonomy ID'),
    '#required' => TRUE,
  ];

  $form['common_name'] = [
    '#type' => 'textfield',
    '#title' => t('Common Name'),
    '#required' => TRUE,
  ];

  $form['is_genome_assembly'] = [
    '#type' => 'textfield',
    '#title' => t('Is the genome assembly already hosted at another genome portal, or is there another genome portal that would also be<br> appropriate to host your dataset (e.g. VectorBase, HGD)?'),
    '#required' => TRUE,
  ];

  $ncbi_options = [
    '' => 'Select',
    'Yes' => 'Yes',
    'No' => 'No',
  ];

  $ncbi_selected = isset($form_state['values']['is_ncbi_submitted']) ? $form_state['values']['is_ncbi_submitted'] : key($ncbi_options);

  $form['is_ncbi_submitted'] = [
    '#type' => 'select',
    '#options' => $ncbi_options,
    '#title' => t('Have you submitted the genome assembly to NCBI, or another INSDC member?'),
    '#default_value' => $ncbi_selected,
    '#required' => TRUE,
  ];

  $assembly_options = [
    '' => 'Select',
    'Yes' => 'Yes',
    'No' => 'No',
  ];
  $assembly_selected = isset($form_state['values']['is_assembly']) ? $form_state['values']['is_assembly'] : key($assembly_options);

  $form['is_assembly'] = [
    '#type' => 'select',
    '#options' => $assembly_options,
    '#title' => t('Is this a re-assembly or new assembly of an existing i5k Workspace organism ?'),
    '#default_value' => $assembly_selected,
    '#required' => TRUE,
  ];

  $fund_options = [
    '' => 'Select',
    'Yes' => 'Yes',
    'No' => 'No',
  ];
  $fund_selected = isset($form_state['values']['involved_in_generation']) ? $form_state['values']['involved_in_generation'] : key($fund_options);

  $form['involved_in_generation'] = [
    '#type' => 'select',
    '#options' => $fund_options,
    '#title' => t('Were you involved in the generation of this genome assembly?'),
    '#default_value' => $fund_selected,
    '#required' => TRUE,
  ];

  $form['description'] = [
    '#type' => 'textarea',
    '#title' => t('Briefly describe your plans for this genome project at the i5k Workspace (e.g. are you interested in community curation?).'),
    '#required' => TRUE,
  ];

  $form['contact_text'] = [
    '#markup' => '<div id="text">Please add your contact information so we can get in touch with you about your genome project.</div>',
  ];

  $form['fullname'] = [
    '#type' => 'textfield',
    '#title' => t('Full Name'),
    '#required' => TRUE,
  ];

  $form['email'] = [
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#required' => TRUE,
  ];

  $form['submit'] = [
    '#type' => 'submit',
    '#value' => t('Submit'),
  ];

  return $form;
}

/**
 * hook_form_validation
 *
 * Email validation
 * taxid is unique for each organism
 */
function request_project_validate($form, &$form_state) {
  if (!valid_email_address($form_state['values']['email'])) {
    form_set_error('email', t('You must enter a valid e-mail address.'));
  }
  $genus = $form_state['values']['genus'];
  $species = $form_state['values']['species'];
  //$is_species_exists = db_select('organism', 'o')->fields('o', array('organism_id'))->condition('o.genus', $genus, '=')->condition('o.species', $species, '=')->execute()->fetchField();

  if (!empty($is_species_exists)) {
    form_set_error('genus', t('The species "' . $genus . ' ' . $species . '" already existed in database.'));
    form_set_error('species');
  }

  if (!empty($form_state['values']['ncbi_taxid']) && !is_numeric($form_state['values']['ncbi_taxid'])) {
    form_set_error('ncbi_taxid', t('NCBI Taxonomy ID should be a number'));
  }

}

/**
 * hook_form_submit
 *
 */
function request_project_submit($form, &$form_state) {
  $values = $form_state['values'];
  // echo "<pre>"; print_r($values);echo "</pre>";exit;

  $created = time();
  // data variable is used to send values to mail message
  $data = [
    'genus' => check_plain($values['genus']),
    'species' => check_plain($values['species']),
    'ncbi_taxid' => check_plain($values['ncbi_taxid']),
    'common_name' => check_plain($values['common_name']),
    'genome_assembly_hosted' => check_plain($values['is_genome_assembly']),
    'is_ncbi_submitted' => check_plain($values['is_ncbi_submitted']),
    'is_assembly' => check_plain($values['is_assembly']),
    'involved_in_generation' => check_plain($values['involved_in_generation']),
    'description' => trim(check_plain($values['description'])),
    'fullname' => trim(check_plain($values['fullname'])),
    'email' => check_plain($values['email']),
    'created' => $created,
  ];
  $result = db_insert('ds_request_project')
    ->fields($data)
    ->execute() or die("Unable to insert data");
  //Send the e-mail to the recipients using the site default language.
  $to_user = $values['email'];
  $from_address = FROM_ADDRESS;
  $send_user = drupal_mail('datasets', 'email_acknowledgement_touser', $to_user, language_default(), $data, $from_address, TRUE);
  if (empty($send_user['result']) || ($send_user['result'] != 1)) {
    drupal_set_message(t('Failed to send e-mail to the submitted user.'));
  }

  //Send email to the admin - when user submitted the data.
  $recipient = FROM_ADDRESS;
  $from = $values['email'];
  $sent = drupal_mail('datasets', 'request_project_email', $recipient, language_default(), $data, $from, TRUE);
  if (empty($sent['result']) || ($sent['result'] != 1)) {
    /*handle send fail, $sent ===false when mail fails, but it won't always recognize a failure*/
    drupal_set_message(t('Failed to send e-mail to the i5k admin.'));
  }
  else {
    drupal_set_message(t('Your Project Request %genus was successfully created. You will receive an email once your request is approved by an administrator.'), [
      '%genus' => $values['genus'],
      '%species' => $values['species'],
    ], $type = 'status');
  }
}