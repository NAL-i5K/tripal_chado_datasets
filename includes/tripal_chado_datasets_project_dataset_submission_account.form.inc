<?php


/**
*  Registration form for project or dataset submission account
*/
function project_dataset_submission_account($form, &$form_state) {
$form['#attached']['css'] = [
drupal_get_path('module', 'datasets') . '/css/project-dataset.css',
];

$form['information_markup'] = [
'#markup' => t('<div id="info_markup">Fill out this form to tell us a bit about your project or dataset. Once we\'ve reviewed your message, we will set you up with an account so you can submit your data to us. This account will NOT give you access to Apollo - For an Apollo account for manual curation, <a href="/web-apollo-registration" target="_NEW">register here</a>.</div>'),
];

$form['name'] = [
'#type' => 'textfield',
'#title' => t('Your Name'),
'#required' => TRUE,
];
$form['email'] = [
'#type' => 'textfield',
'#title' => t('email'),
'#required' => TRUE,
];
$form['affiliation'] = [
'#type' => 'textfield',
'#title' => t('Affiliation'),
'#required' => TRUE,
];

$form['content'] = [
'#type' => 'textarea',
'#title' => t('Tell us about the data you want to submit or the project you want to start'),
'#required' => TRUE,
];


// Enabling honeypot to avoid spam
honeypot_add_form_protection($form, $form_state, [
'honeypot',
'time_restriction',
]);
$form['captcha'] = [
'#type' => 'captcha',
'#captcha_type' => 'datasets/Math',
];


$form['submit'] = [
'#type' => 'submit',
'#value' => t('Submit'),
];


return $form;

}


/**
*  hook_form_validate()
*/
function project_dataset_submission_account_validate($form, & $form_state) {
$values = $form_state['values'];
$name = $values['name'];

if (!preg_match('!^[\w @-]*$!', $name)) {
form_set_error('name', t('Your Name can only contains numbers, letters @  -_.'));
}

if (!valid_email_address($values['email'])) {
form_set_error('email', t('You must enter a valid e-mail address.'));
}


$check_name_unique = db_select('ds_project_dataset_account')
->fields('ds_project_dataset_account', ['psid'])
->condition('name', db_like($values['name']), 'LIKE')
->range(0, 1)
->execute()
->fetchField();
if (!empty($check_name_unique)) {
form_set_error('name', t('The name %name is already taken.', ['%name' => $values['name']]));
}

$check_email_unique = db_select('ds_project_dataset_account')
->fields('ds_project_dataset_account', ['psid'])
->condition('email', db_like($values['email']), 'LIKE')
->range(0, 1)
->execute()
->fetchField();
if (!empty($check_email_unique)) {
form_set_error('email', t('The e-mail address %email is already taken.', ['%email' => $values['email']]));
}

$affiliation = $values['affiliation'];
if (!preg_match('!^[\w @-]*$!', $affiliation)) {
form_set_error('affiliation', t('Your
Affiliation can only contains numbers, letters @  - _.'));
}

}

/**
* hook_form_submit()
*/
function project_dataset_submission_account_submit($form, & $form_state) {
$values = $form_state['values'];
// echo "<pre>"; print_r($values);echo "</pre>";

$created = time();
// data variable is used to send values to mail message
$data = [
'name' => check_plain($values['name']),
'email' => check_plain($values['email']),
'affiliation' => check_plain($values['affiliation']),
'content' => check_plain($values['content']),
'created' => $created,
];

// drupal_write_record('ds_request_project', $data);
$result = db_insert('ds_project_dataset_account')
->fields($data)
->execute() or die("Unable to insert data");
if ($result > 0) {
// E-mail address of the sender: as the form field is a text field.
$from = $values['email'];
$to_address = FROM_ADDRESS;

// Send the e-mail to the recipients using the site default language.
$sent = drupal_mail('datasets', 'register_project_submission_account', $to_address, language_default(), $values, $from, TRUE);

if (empty($sent['result']) || ($sent['result'] != 1)) {
/*handle send fail, $sent ===false when mail fails, but it won't always recognize a failure*/
drupal_set_message(t('Failed to send e-mail.'));
}
else {
drupal_set_message(t('Thanks for registering for project or data submission account. Once we\'ve reviewed your data, we will set you up with an account so you can submit your data to us.'));
}
}
}
