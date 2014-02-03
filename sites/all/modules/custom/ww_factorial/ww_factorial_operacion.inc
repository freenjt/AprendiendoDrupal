<?php

function operation_factorial_form($form, &$form_state){
$form['factorial'] = array(
  '#type' => 'fieldset', 
  '#title' => t('Set factorial'), 
  //'#weight' => 5, 
  '#collapsible' => TRUE, 
  '#collapsed' => FALSE,
);

// Campo de texto donde va el n�mero al cual se le quiere sacar el factorial.
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

function operation_factorial_submit($form,&$form_state){
  $resultado=1;
 // dpm($form_state,'form_state');
  if(!empty($form_state)){
    $numero=$form_state['values']['numero'];
    for($i=$numero; $i>0; $i--){
      $resultado=$resultado*$i;      
    }
  }
drupal_set_message(t('Result factorial= %resultado', array('%resultado' => $resultado)));
}