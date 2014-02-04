<?php
/**
 * Implements formulario persona
 */
function people_form($form, &$form_state){
  $form['contact'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Form contact'), 
    //'#weight' => 5, 
    '#collapsible' => TRUE, 
    '#collapsed' => FALSE,
  );
  $form['contact']['name'] = array(
    '#type' => 'textfield', 
    '#title' => t('Write name of user.'), 
    //'#default_value' => variable_get(''), 
    '#size' => 60, 
    '#maxlength' => 70, 
    '#required' => TRUE,
  );
  $form['contact']['email'] = array(
    '#type' => 'textfield', 
    '#title' => t('Write email of user.'), 
    '#description' => t('Format email with @'),
    '#size' => 60, 
    '#maxlength' => 70, 
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit', 
    '#value' => t('Save information'),
    '#submit' => array('people_form_submit'),
    '#validate' => array('people_form_validate'),
  );
  
  return $form;
}

/**
 * Implements form_validate.
 */
function people_form_validate($form, &$form_state) {
  if(!empty ($form_state)){
    //$verificar=check_email_address();
    if(!filter_var($form_state['values']['email'], FILTER_VALIDATE_EMAIL)){
      form_set_error('email', t('Error in format of email.'));
    }
    $resultado=db_query('select p.email_people from {persona} p');
    foreach ($resultado as $record) {
      if($record->email_people == $form_state['values']['email']){
        form_set_error('email', t('Error email repetido'));
      }
    }
  }
}
/**
 * Implements form_submit.
 */
function people_form_submit($form, &$form_state){
  try {
    if(!empty ($form_state)){
      $registro=db_insert('persona')
      ->fields(array(
      'name_people' => $form_state['values']['name'],'email_people' => $form_state['values']['email'],
      ))->execute();  
    }
    drupal_set_message(t('The form has been submitted. name="@name , email=@email',
    array('@name' => $form_state['values']['name'], '@email' => $form_state['values']['email'])));
  } catch (Exception $exc) {
    drupal_set_message(t('The form has not been submitted for @error',
    array('@error' => $exc->getTraceAsString())),'error');
    //echo $exc->getTraceAsString();
  }

}