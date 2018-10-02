<?php

/**
 * Implement hook_mail
 *
 * @param $key - Valid keys are:
 *  confirm_user_organism_submission
 *  request_project_approved_email
 *  register_project_submission_account
 *  submit_dataset_email
 *
 * @param $message
 * @param $params
 */

function tripal_chado_datasets_mail($key, &$message, $params) {
  global $base_url, $base_path;
  $language = $message['language'];


  $name = isset($params['fullname']) ? $params['fullname'] : $params['cordinator_name'];

  $site_name = variable_get('site_name', 'Drupal');

  $variables = [
    '!organism' => $params['genus'] . ' ' . $params['species'],
    '!user' => $name,
    '!form' => url($_GET['q'], ['absolute' => TRUE]),
  ];


  $filename_md5sum = isset($params['SHA512']) ? $params['SHA512'] : NULL;

  if (isset($params['upload']) && !empty($params['upload'])) {
    $file = file_load($params['upload']);
    $filename_md5sum = $file->filename . ' & ' . $filename_md5sum;
  }
  switch ($key) {
    //Sent by tripal_chado_datasets_request_project_submit.
    //(Create chado organism request ackowledgement).

    case 'confirm_user_organism_submission':

      $subject = t('Genome project submission inquiry at ' . $site_name);
      $message['subject'] = $subject;

      $message['body']['preamble'] = t('Thank you for your interest in submitting a genome project to ' . $site_name . '!  Your request has been received, and a site administrator will contact you soon.');
      $message['body']['signature'] = '<br/><br/>Best, <br/><br/>The ' . $site_name . 'team';
      $message['headers']['Content-Type'] = 'text/html; charset=UTF-8; format=flowed';
      break;

    case 'request_project_email':

      //TODO this looks off to me... shouldnt it be !genus instead of $params['genus'] if we apass in teh $variables array into t()?

      $message['subject'] = t('New project request for "' . $params['genus'] . ' ' . $params['species'] . '"', $variables, ['langcode' => $language->language]);
      //$variables required even if not used to get $language in there
      //the email body is here, inside the $message array
      $message['body'][] = t($params['fullname'] . " requested for new project.<br><br><b>Below are the details:</b>", $variables, ['langcode' => $language->language]);
      $message['body'][] = '<br><b>Genus:</b> ' . $params['genus'] . '
<br><b>Species:</b> ' . $params['species'] . '
<br><b>NCBI Taxid: </b>' . $params['ncbi_taxid'] . '
<br><b>Common Name: </b>' . $params['common_name'] . '
<br><b>Is the genome assembly already hosted at another genome portal, or is there another genome portal that would also be appropriate to host your dataset: </b>' . $params['genome_assembly_hosted'] . '
<Br><b>Have you submitted the genome assembly to NCBI, or another INSDC member?: </b>' . $params['is_ncbi_submitted'] . '
<br><b>Is this a re-assembly or new assembly of an existing organism on the site? :</b> ' . $params['is_assembly'] . '
<br><b>Were you involved in the generation of this genome assembly? : </b>' . $params['involved_in_generation'];

      $message['body'][] = '<br><b>Briefly describe your plans for this genome project at ' . $site_name . ' </b>' . $params['description'] . '  <br><b>Full Name: </b>' . $params['fullname'] . '
<br><b>Email:</b> ' . $params['email'];
      $message['body'][] = "<br><br>To view more click the <a href='" . $GLOBALS['base_url'] . "/admin/structure/datasets'>Admin</a> Link";

      $message['body'][] = '<br><BR>Best,<br><br>' . $site_name . ' team';
      $message['headers']['Content-Type'] = 'text/html; charset=UTF-8; format=flowed';

      break;

    case 'request_project_approved_email':
      //Let users know their project was approved.

      $message['subject'] = t($site_name . ' Project Request Approved for "' . $params['genus'] . ' ' . $params['species'] . '"', $variables, ['langcode' => $language->language]);
      $message['body'][] = "Dear " . $params['fullname'] . ", <br><br>We have approved your project request for " . $params['genus'] . ' ' . $params['species'] . ".";
      $message['body'][] = '<br>You can submit your datasets. Please visit <a href=' . $base_url . '"/datasets/submit-a-dataset"> ' . $base_url . '/datasets/submit-a-dataset</a> to submit.<Br><br>Best,<br><bR>' . $site_name . ' team';
      $message['headers']['Content-Type'] = 'text/html; charset=UTF-8; format=flowed';
      break;

    case 'register_project_submission_account':
      //sent in tripal_chado_datasets_project_submission_account_submit
      //request an account to submit data.

      $message['subject'] = t("" . $params['name'] . " registered for project or data submission account", $variables, ['langcode' => $language->language]);
      $message['body'][] = "Below are the user details<br><br> ";
      $message['body'][] = "<b>Name:</b> " . $params['name'] . "<br>";
      $message['body'][] = "<b>Email:</b> " . $params['email'] . "<br>";
      $message['body'][] = "<b>Affiliation:</b> " . $params['affiliation'] . "<br>";
      $message['body'][] = "<b>Tell us about the data you want to submit or the project you want to start:</b> " . $params['content'] . "<br>";
      $message['body'][] = "<br>To create an user account go to <a href='" . $base_url . $base_path . "/admin/people'>" . $base_url . " /admin/people</a><br> Then click \"Add User\"<br><br>";
      $message['headers']['Content-Type'] = 'text/html; charset=UTF-8; format=flowed';
      break;

    case 'submit_dataset_email':
      $message['subject'] = t('Submit dataset for ' . $params['organism'], $variables, ['langcode' => $language->language]);
      $message['body'][] = t("Dear " . $params['name'] . ", <br><br> Thank you for submitting  a dataset for " . rtrim($params['organism']) . ". We will review your submission and send you information on how to upload your dataset as soon as possible.", $variables, ['langcode' => $language->language]);
      $message['body'][] = "<h1>Submission details:</h1>";
      $message_body = "<b>Co-ordinator information:</b><br>";
      $message_body .= "Name: " . $params['name'] . "<br>";
      $message_body .= "Email: " . $params['email'] . "<br><br>";
      $message_body .= "<b>Project Background:</b><br>";
      $message_body .= "Organism: " . $params['organism'] . "<br>";
      $message_body .= "Common name: " . $params['common_name'] . "<br>";
      $message_body .= "Organism: " . $params['organism'] . "<br>";
      $message_body .= "Submitter Name: " . $params['name'] . "<br>";
      $message_body .= "Email Address: " . $params['email'] . "<br>";
      $message_body .= "Program: " . $params['program'] . "<br>";
      $message_body .= "Version: " . $params['version'] . "<br>";
      $message_body .= "Additional Information: " . $params['additional_info'] . "<br>";
      $message_body .= "Methods Citaion (DOI): " . $params['methods_citation'] . "<br>";
      $message_body .= "Dataset name: " . $params['dataset_name'] . "<br>";
      $message_body .= "Dataset Version: " . $params['dataset_version'] . "<br>";
      $message_body .= "Should we make this file available for download in our Data Downloads section?: " . $params['is_download'] . "<br>";
      $message_body .= "Is the dataset published?: " . $params['dataset_is_publish'] . "<br>";
      if ($params['dataset_is_publish'] == 'Yes') {
        $message_body .= "If yes: " . $params['dataset_publish_field_data'] . "<br>";
      }
      elseif ($params['dataset_is_publish'] == 'No') {
        $message_body .= "If no: " . $params['dataset_publish_field_data'] . "<br>";
      }

      if ($params['dataset_type'] == 'Assembly') {
        $message_body .= "<h3>Genome assembly information:</h3>";
        $message_body .= "Project description to display in your organism page: " . $params['description'] . "<br>";
        $message_body .= "Will you manually curate this assembly?: " . $params['is_curate_assembly'] . "<br>";
        if (isset($params['is_curate_assembly']) && $params['is_curate_assembly'] == 'Yes') {
          $message_body .= "Co-ordinator name: " . $params['manual_curation_name'] . "<bR>";
          $message_body .= "Co-ordinator Email: " . $params['manual_curation_email'] . "<bR>";
          $message_body .= "Do you need assistance developing an Official Gene Set? " . $params['need_assistance'] . "<bR>";
          if (isset($params['need_assistance']) && ($params['need_assistance'] == 'Yes')) {
            $message_body .= "Reason: " . $params['reason'] . "<bR>";
          }

          $message_body .= "<b>Curation time frame:</b><br>";
          if ($params['no_timeframe'] == 1) {
            $message_body .= "There is no set time frame for curation";
          }
          else {
            $from_dd = explode('-', $params['time_from']);
            $from_dd_format = $from_dd[1] . '-' . $from_dd[2] . '-' . $from_dd[0];
            $message_body .= "From Date: " . $from_dd_format . "<bR>";
            $to_dd = explode('-', $params['time_to']);
            $to_dd_format = $to_dd[1] . '-' . $to_dd[2] . '-' . $to_dd[0];
            $message_body .= "To Date: " . $to_dd_format . "<bR>";
          }
        }

        $message_body .= "Geographic location (latitude and longitude): " . $params['assembly_geo_location'] . "<br>";
        $message_body .= "Tissues/Life stage included: " . $params['assembly_tissues_located'] . "<br>";
        if (!empty($params['assembly_gender'])) {
          $message_body .= "Sex: " . $params['assembly_gender'] . "<br>";
          if ($params['assembly_gender'] == 'NA') {
            $message_body .= "Other (sex): " . $params['other_gender'] . "<br>";
          }
        }
        $message_body .= "Strain: " . $params['data_source_strain'] . "<br>";
        $message_body .= "Other notes: " . $params['data_source_notes'] . "<br>";
        $message_body .= "Sequencing method: " . $params['data_source_seqplatform'] . "<br>";
        $message_body .= "NCBI/INSDC Genome Assembly accession: " . $params['assembly_data_source_url'] . "<br>";
        $message_body .= "Other notes: " . $params['additional_other_notes'] . "<br>";
      }

      if ($params['dataset_type'] == 'Gene prediction') {

        $message_body .= "<h3>Gene set information:</h3>";
        $message_body .= "Descriptive track name for JBrowse and Apollo: " . $params['gene_set_descriptive_track'] . "<br>";
        $message_body .= "Is this an Official Gene Set?: " . $params['is_ogs'] . "<br>";
      }

      if ($params['dataset_type'] == 'Mapped dataset') {
        $message_body .= "<h3>Mapped dataset information:</h3>";
        $message_body .= "Geographic location (latitude and longitude): " . $params['mapped_dataset_geo_location'] . "<br>";
        $message_body .= "Tissues/Life stage included: " . $params['mapped_dataset_tissues_located'] . "<br>";
        if (!empty($params['mapped_gender'])) {
          $message_body .= "Sex: " . $params['mapped_gender'] . "<br>";
          if ($params['mapped_gender'] == 'NA') {
            $message_body .= "Other (sex): " . $params['other_gender'] . "<br>";
          }
        }
        $message_body .= "Sequencing method: " . $params['sequence_platform'] . "<br>";
        $message_body .= "Descriptive track name for JBrowse and Apollo: " . $params['mapped_dataset_descriptive_track'] . "<br>";
        $message_body .= "NCBI SRA accession number(s): " . $params['mapped_dataset_data_source_url'] . "<br>";
      }
      $message_body .= "<br><b>Filename & SHA512 </b>:";
      $message_body .= "<br>" . $filename_md5sum . "<br>";
      $message['body'][] = $message_body;
      $message['body'][] = '<br><br>Best,<br><br>' . $site_name . ' team';
      $message['headers']['Content-Type'] = 'text/html; charset=UTF-8; format=flowed';
      break;
  }
}