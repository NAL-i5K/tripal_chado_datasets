<?php

/**
 * @file: This form is for viewing, approving, or rejecting chado analysis
 *   (dataset) submissions.  In the future it should/could be generecized to
 *   handle organisms or any submission instead.
 */

/**
 * @param $submission_id : ds_submissions.oa_id
 */
function tripal_chado_datasets_edit_approve_dataset_form($form, &$form_state, $submission_id) {

  $query = db_select('ds_submissions', 'os');
  $query->fields("os");
  $query->condition("oa_id", $submission_id);
  $result = $query->execute()->fetchAll();

  //We will just loop and print key value pairs, so clean up any key names or values here!

  $result = $result[0];


  $form['details'] = [
    '#type' => 'fieldset',
    '#tree' => true, //this lets us tell our properties apart from drupal for, values.
    '#access' => false,//dont show to users.
  ];

  //replace organism id with common name in chado.
  $organism_id = $result->organism;
  $organism = chado_get_organism(['organism_id' => $organism_id]);
  $result->organism = $organism->common_name;
  $form['oa_id'] = ['#type' => 'value', '#value' => $submission_id];
  unset($result->oa_id);

  $form['introduction'] = ['#markup' => '<h3>Request Review</h3><p>The form submitted by the user is summarized in the table below.  You can press the <b>Approve</b> button, which will create the record in Chado and notify the user.  The reject button will change the status of this request but not create a record or notify the user.</p>'];

  $rows = [];

  $header = ['Key', 'Value'];

  $legend = tripal_chado_dataset_key_legend();

  foreach ($result as $key => $value) {
    if (!$value) {//ignore unset keys
      continue;
    }
    $display_key = $key;
    if (isset($legend[$key])) {
      $display_key = $legend[$key];
    }
    $rows[] = [$display_key, $value];
    $form['details'][$key] = ['#type' => 'value', '#value' => $value];

  }


  $output = theme('table', ['header' => $header, 'rows' => $rows]);

  $form['table'] = [
    '#type' => 'markup',
    '#title' => t('Submission Details'),
    '#markup' => $output,
  ];

//  $form['submit'] = ['#type' => 'submit', '#value' => "Approve"];
//  $form['reject'] = [
//    '#type' => 'submit',
//    '#value' => "Reject",
//    '#submit' => ['tripal_chado_dataset_reject_record'],
//  ];

  return $form;
}

//Submits the chado record.  For now, this is just analysis.
function tripal_chado_datasets_edit_approve_dataset_form_submit($form, &$form_state) {

  $record = dpm($form_state['values']['details']);

  //create analysis

  $values = [
    'name' => $record['dataset_name'],
    'description' => 'We dont ask this?  Do we generate it?',
    'program' => $record['program'],
    'programversion' => $record['version'],
    'algorithm' => 'we dont do ask this',
    'sourcename' => 'we dont ask this',
    'sourceuri' => 'we dont ask this',
  ];

  $options = [];
  //chado_insert_record('analysis', $values, []);

  //add props

  //update the oa_id

}


/**
 * REJECT the proposed record.
 */
function tripal_chado_dataset_reject_record($form, &$form_state) {

  tripal_set_message('Sorry, not implemented yet!  coming soon', TRIPAL_ERROR);
}

/**
 * A legend for key/value pairs stored in this module.  So far this handles
 * analysis only.  Doesnt include all keys necessarily!
 *
 * @return array
 */
function tripal_chado_dataset_key_legend() {

  $legend = [
    'name' => 'Submitter Name',
    'email' => 'Email',
    'affiliation' => 'Affiliation',
    'other_methods' => 'Methods Citation (DOI)',
    'is_download' => 'File Available for Data Downloads Section?',
    'dataset_is_publish' => "Dataset is Published?",
    'mapped_dataset_geo_location' =>
      'Geographic Location (latitude and longitude)',
    //TODO: not dry.
    //Note redundancy here of mapped_dataset geo location and assembly geo location....
    'assembly_geo_location' => 'Geographic Location (latitude and longitude)',
    'mapped_dataset_tissues_located' => 'Tissues/Life Stage Included?',
    'sequence_platform' => 'Sequencing Method (e.g. Illumina Hi-Seq)',
    'mapped_data_source_url' => 'Descriptive track name for JBrowse and Apollo',
    'assembly_accession' => 'NCBI SRA accession number',
  ];
  //skip simple to_upper for now, maybe there's a smart way to do that.
  return $legend;
}