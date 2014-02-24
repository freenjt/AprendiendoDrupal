<?php

/**
 * @file
 * This file content information about forms.
 *
 * Contiene todos los formularios y sus debidas validaciones y envios.
 */

/**
 * Implements hook_form().
 */
function subir_foto_form($form_state) {
  global $user;
  //capturar_dato();
  $query = consultar_foto();
  $form['contact'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Form of user'), 
    '#collapsible' => TRUE, 
    '#collapsed' => FALSE,
  );
  if ($query == '') {
  $file = file_load(variable_get('valor_defecto', 0));
  }
  else {
    $file =file_load($query['fid']);
  }
  $image = theme('image', array('path' => $file->uri, 'alt' => 'Alt text', 'height' => 100));
  
  $form['contact']['imagen']=array(
    '#markup' => $image
  ); 
  $form['contact']['file'] = array(
    '#type' => 'file',
    '#title' => t('Image'),
    '#description' => t('Upload a file, allowed extensions: jpg, jpeg, png, gif'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save photo'),
  );
  return $form;
}

function subir_foto_form_validate($form, &$form_state) {
  $file = file_save_upload('file', array(
    'file_validate_is_image' => array(), // Validates file is really an image.
    'file_validate_extensions' => array('png gif jpg jpeg'), // Validate extensions.
  ));
  // If the file passed validation:
  if ($file) {
    // Move the file, into the Drupal file system
    if ($file = file_copy($file, 'public://')) {
      // Save the file for use in the submit handler.
      dpm($file, 'file');
      $form_state['storage']['file'] = $file;
    }
    else {
      form_set_error('file', t('Failed to write the uploaded file to the site\'s file folder.'));
    }
  }
  else {
    form_set_error('file', t('No file was uploaded.'));
  }
}
            
function subir_foto_form_submit($form, &$form_state) {
  global $user;
  
  $file = $form_state['storage']['file'];
  // We are done with the file, remove it from storage.
  unset($form_state['storage']['file']);
  // Make the storage of the file permanent
  $file->status = FILE_STATUS_PERMANENT;
  // Save file status.
  file_save($file);
  
  $query=consultar_foto();
  
  if ($query == '') {
  db_insert('datos_usuario')
    ->fields(array(
      'id_user' => $user->uid,
      'fid' => $file->fid,  
      ))
    ->execute();
  }
  else{
    db_update('datos_usuario')
      ->fields(array('fid' => $file->fid ))
      ->condition('id_user', $user->uid)
      ->execute();
    db_delete('file_managed')
      ->condition('fid', $query['fid'])
      ->execute();
  }
  // Set a response to the user.
  drupal_set_message(t('The form has been submitted and the image has been saved, filename: @filename.', array('@filename' => $file->filename)));
}

/**
 * Funcion que me permite saber si un usuario tiene foto registrada o no.
 * 
 * @global type $user
 * @return array 
 */
function consultar_foto() {
  global $user;
  if (db_table_exists('datos_usuario')) {
  $query=db_select('datos_usuario', 'du')
    ->fields('du')
    ->condition('id_user', $user->uid)
    ->execute()
    ->fetchAssoc();
  return $query;
}
}

// function usuario_form_configure($form, &$form_state){


//   $form['submit'] = array('#type' => 'submit', '#value' => t('Import image'));
//   // $query = db_select('datos_usuario', 'du')
//   //   ->fields('du')
//   //   ->execute()
//   //   ->fetchAssoc();

//   // if($query == ''){
//   //   db_insert('datos_usuario')
//   //   ->fields(array(
//   //     ''));  
//   // }

//   return $form;
  
// }

// function usuario_form_configure_submit($form, &$form_state){

// }