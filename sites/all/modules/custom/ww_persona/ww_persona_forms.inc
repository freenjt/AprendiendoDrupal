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
  valida_formularios($form_state, 1);
}

function valida_formularios(&$form_state, $opcion=0){
    if(!empty ($form_state)){
    //$verificar=check_email_address();
    if(!filter_var($form_state['values']['email'], FILTER_VALIDATE_EMAIL)){
      form_set_error('email', t('Error in format of email.'));
    }
    //opcion 1:No recomendada
    //$resultado=db_query('select p.email_people from {persona} p');
    //opcion 2:Recomendada
    $resultado=db_select('persona','p')
      ->fields('p',array('email_people'))
      ->condition('email_people',$form_state['values']['email'],'=')
      ->execute();
    if($opcion == 1){

      dpm($resultado,'resultado');
        //$resultado;
      foreach ($resultado as $record) {
      //if($record->email_people == $form_state['values']['email']){
        form_set_error('email', t('Error email repetido'));
      //}
      }
    }
    elseif ($opcion == 0) {
      $result=db_select('persona','p')
        ->fields('p')
        ->condition('id_people',$form_state['values']['oculto'],'=')
        ->execute()
        ->fetchAssoc();
      $compara=$resultado->fetchAssoc();
      //dpm($compara,'compara');
      if($compara != '' && $compara['email_people'] != $result['email_people']){
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
    $form_state['redirect'] = 'listar/persona';
  } catch (Exception $exc) {
    drupal_set_message(t('The form has not been submitted for @error',
    array('@error' => $exc->getTraceAsString())),'error');
    //echo $exc->getTraceAsString();
  }
}

/**
 * Implements formulario para editar persona
 */
function people_edit($form, &$form_state, $id){
  //dpm($id,'id');
   $valor=$id;
    $resultado=db_select('persona','p')
      ->fields('p')
      ->condition('id_people',$valor,'=')
      ->execute()
      ->fetchAssoc();
    //dpm($resultado,'resultado');
    $form['contact'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Form contact'), 
    //'#weight' => 5, 
    '#collapsible' => TRUE, 
    '#collapsed' => FALSE,
  );
    $form['contact']['name'] = array(
    '#type' => 'textfield', 
    '#title' => t('Edit name of user.'), 
    '#default_value' => $resultado['name_people'], 
    '#size' => 60, 
    '#maxlength' => 70, 
    '#required' => TRUE,
  );
    $form['contact']['email'] = array(
    '#type' => 'textfield', 
    '#title' => t('Edit email of user.'), 
    '#description' => t('Format email with @'),
    '#default_value' => $resultado['email_people'], 
    '#size' => 60, 
    '#maxlength' => 70, 
    '#required' => TRUE,
  );
  $form['contact']['oculto'] = array(
    '#type' => 'hidden',
    '#value' => $valor
  );
    $form['submit'] = array(
    '#type' => 'submit', 
    '#value' => t('Save information'),
    '#submit' => array('people_edit_submit'),
    '#validate' => array('people_edit_validate'),
  );
    //drupal_set_message(t('this is valor=@id',array('@id'=>$id_people)));
    return $form;
}

/**
 * Implements form validate
 */
function people_edit_validate($form, $form_state){
  valida_formularios($form_state);
}

/**
 * Implements form submit
 */
function people_edit_submit($form, &$form_state){
    try {
    if(!empty ($form_state)){
      $registro=db_update('persona')
      ->fields(array(
      'name_people' => $form_state['values']['name'],'email_people' => $form_state['values']['email'],
      ))
      ->condition('id_people',$form_state['values']['oculto'],'=')
      ->execute();  
    }
    drupal_set_message(t('The form has been submitted. name="@name , email=@email',
    array('@name' => $form_state['values']['name'], '@email' => $form_state['values']['email'])));
    $form_state['redirect']='listar/persona';
  } catch (Exception $exc) {
    drupal_set_message(t('The form has not been submitted for @error',
    array('@error' => $exc->getTraceAsString())),'error');
    //echo $exc->getTraceAsString();
  }
}

/**
 * Implements metodo para eliminar registro
 */
function people_delete($form, $form_state, $id){
  $valor=$id;
  $resultado=db_select('persona','p')
    ->fields('p')
    ->condition('id_people',$id,'=')
    ->execute()
    ->fetchAssoc();
  $form['contact'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Form contact'),
    '#description' => t('You wish delete the record of '.$resultado['name_people'].'?'),  
    //'#weight' => 5, 
    '#collapsible' => TRUE, 
    '#collapsed' => FALSE,
  ); 
  $form['valor_oculto']=array(
    '#type' => 'value',
  /* @var $id type */
    '#value' => $valor
  );
  $form ['contact']['submit'] = array(
    '#type' => 'submit', 
    '#value' => t('Delete'));
   return $form;   
}

/**
 * Implements form_submit
 */
function people_delete_submit($form, &$form_state){
  try {
    db_delete('persona')
    ->condition('id_people',$form_state['values']['valor_oculto'],'=')
    ->execute();
    drupal_set_message(t('People with id=@id has been deleted.', array('@id' => $form_state['values']['valor_oculto'])));
    $form_state['redirect'] = 'listar/persona';
  } catch (Exception $exc) {
    drupal_set_message(t('The form has not been submitted for @error',
    array('@error' => $exc->getTraceAsString())),'error');
  }

}