<?php
/**
 * Formulario form
 */
function operation_factorial_form($form, &$form_state){
$form['factorial'] = array(
  '#type' => 'fieldset', 
  '#title' => t('Set factorial'), 
  //'#weight' => 5, 
  '#collapsible' => TRUE, 
  '#collapsed' => FALSE,
);

// Campo de texto donde va el número al cual se le quiere sacar el factorial.
$form ['factorial']['numero'] = array(
  '#type' => 'textfield', 
  '#title' => t('Write number'), 
  //'#default_value' => $node->title, 
  '#size' => 60, 
  '#maxlength' => 50, 
  '#required' => TRUE,
);

// Boton de envio para calculara el factorial.
$form['submit'] = array(
  '#type' => 'submit',
  '#value' => t('Factorial'),
  '#submit' => array('operation_factorial_submit'),
  );
return $form;
}

function factorial_configuracion(){
    $form['titulo_factorial'] = array(
    '#title' => t('Title of the form'),  
    '#type' => 'textfield',
    '#default_value' => variable_get('titulo_factorial', 'Calculando factorial'),  
    '#required' => TRUE,  
  );
  /*
  $form['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Guardar configuraciÃ³n',
  );
  $form['#submit'][] = 'saludar_configuracion_submit';  */
  return system_settings_form($form);
}

function operation_factorial_submit($form,&$form_state){
  $resultado=1;
  // dpm($form_state,'form_state');
  if(!empty($form_state)){
    $numero=$form_state['values']['numero'];
    for($i=$numero; $i>0; $i--){
      $resultado=$resultado*$i;      
    }
  }
<<<<<<< HEAD
  drupal_set_message(t('Result factorial= %resultado', array('%resultado' => $resultado)));
=======
drupal_set_message(t('Result factorial= %resultado', array('%resultado' => $resultado)));
>>>>>>> master
}

/**
 * Implements hook_form
 */
function list_numbers_form($form, &$form_state){
  $form['contact'] = array(
  '#type' => 'fieldset', 
  '#title' => t('Contact settings'), 
  '#weight' => 5, 
  '#collapsible' => TRUE, 
  '#collapsed' => FALSE,
);
  $form['contact']['number'] = array(
  '#type' => 'textfield', 
  '#title' => t('Digit the number'), 
  //'#default_value' => $node->title, 
  '#size' => 60, 
  '#maxlength' => 128, 
  '#required' => TRUE,
);
  $form['contact']['submit'] = array(
      '#type' => 'submit', 
      '#value' => t('Set numbers'));
  return $form;
}

function list_numbers_form_validate($form, &$form_state){
  if(!is_numeric($form_state['values']['number'])){
    form_set_error('number','el dato ingresado no es numerico');
  }
}
function list_numbers_form_submit($form, &$form_state){
 batch_set(asignando_operaciones($form_state['values']['number']));
}

function asignando_operaciones($id){
<<<<<<< HEAD
=======
  $path = drupal_get_path('module', 'ww_factorial');
>>>>>>> master
  $operations = array();
   drupal_set_message(t('Creating an array of @num operations', array('@num' => $id)));
  for($i=0; $i<$id; $i++){
    $operations[]=array('operacion_numeros', array());
    //$operations[] = array('operacion_numeros', array($nid, t('(Operation @operation)', array('@operation' => $i))));
  }
  $batch = array(
    'operations' => $operations,
    'finished' => 'asignando_operaciones_finished',
    //'error_message' => t('Batch has encountered an error.'),
<<<<<<< HEAD
    'file' => 'ww_factorial_operacion.inc',
=======
    'file' => $path.'/ww_factorial_operacion.inc',
>>>>>>> master
  );
  //dpm($batch,'batch');
  return $batch;
}

function operacion_numeros(&$context){
  $str="";
  $context['message'] = 'entro ' . rand();
  //dpm($context,'context');
  for($i=1; $i<=10000; $i++){
    $str .= $i . "-";
    
  }
  $context['results'] []= $str;
  drupal_set_message(t('El resultado= @resultado', array('@resultado' => $str)));
}

function asignando_operaciones_finished($success, $results, $operations){
    if($success){
  drupal_set_message(t('Numero de resultados= @resultado', array('@resultado' => count($results))));
  }
  else{
  $error_operation = reset($operations);
  drupal_set_message(t('no muestra resultados'));
}
}