<?php
/**
 * Implements formulario persona
 */
function people_form($form, &$form_state){
   drupal_add_library("ww_campos", "chosen");
  $path = drupal_get_path("module", "ww_campos");
   drupal_add_js($path."/agregando_chosen.js", array('scope' => 'footer'));
   
  $options = range(0, 100);
  $form['lista'] = array(
    '#type' => 'select', 
    '#title' => t('List'), 
    '#options' => drupal_map_assoc($options),
    '#attributes' => array('class' => array('chosen-select-deselect'), 'style' => '300px'),
  );
    
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

      //dpm($resultado,'resultado');
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

/**
 * Formulario de arrastrar tabla
 */
function table_dragg_form($form, &$form_state){
  //dpm($form_state, 'form_state');
  $limit_peoples = variable_get('limit_list_people');
  $verificar_nombre=(isset($form_state['input']['name'])) ? $form_state['input']['name'] : '';
  $form['criteria'] = array(
    '#type' => 'fieldset',
    '#title' => 'Search Criteria',
    '#attributes' => array('class' => array('container-inline')),
  );

  $form['criteria']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name people'),
    '#value' => (isset($form_state['input']['name'])) ? $form_state['input']['name'] : "",
    '#size' => 40,
    '#maxlength' => 20,
  );
$path = drupal_get_path('module', 'ww_persona');
    $form['#attached']['css'][] = $path . '/css/mi_estilo.css'; 
drupal_add_css($path . '/css/mi_estilo.css');
  $form['criteria']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'search',
    //'#theme' => 'button',
    '#attributes' => array('class' => array('botones')),
    '#submit' => array('my_search_submit'),
  );
  
  $form['example_items']['#tree'] = TRUE;
  //$result = db_query('SELECT id, name, description, weight FROM {tabledrag_example} WHERE id < 8 ORDER BY weight ASC');
  $result=db_select('persona' , 'p')
    ->fields('p')
    ->orderBy('weight', 'ASC')
    ->extend('PagerDefault')
    ->limit($limit_peoples);
    if($verificar_nombre != ''){
    $result = $result->condition('name_people', '%' . db_like($verificar_nombre) . '%', 'LIKE');
    
    }
   $result = $result -> execute();

   if($verificar_nombre != '' && count($result->fetchAll())<=0){
     $arreglo=explode(" ", $verificar_nombre, 7);
     $result = db_select('persona', 'p')
    ->extend('PagerDefault') 
    ->fields('p')
    ->orderBy('weight', 'ASC')
    ->limit($limit_peoples);
     $or = db_or();
    for($i=0; $i<count($arreglo); $i++){
      $or=$or->condition('name_people', '%' . db_like($arreglo[$i]) . '%', 'LIKE');
      // $query = $query->condition('name_people', '%' . db_like($arreglo[$i]) . '%', 'LIKE')->condition($or);
    }
     $result = $result->condition($or)->execute(); 
   }
    foreach ($result as $item) {
//dpm($item, 'item');
    // Create a form entry for this item.
    //
    // Each entry will be an array using the the unique id for that item as
    // the array key, and an array of table row data as the value.
    $form['example_items'][$item->id_people] = array(

      // We'll use a form element of type '#markup' to display the item name.
      'name' => array(
        '#markup' => check_plain($item->name_people),
      ),

      // We'll use a form element of type '#textfield' to display the item
      // description, which will allow the value to be changed via the form.
      // We limit the input to 255 characters, which is the limit we set on
      // the database field.
      'email' => array(
//        '#type' => 'textfield',
//        '#default_value' => check_plain($item->description),
//        '#size' => 20,
//        '#maxlength' => 255,
        '#markup' => check_plain($item->email_people),  
      ),

      // The 'weight' field will be manipulated as we move the items around in
      // the table using the tabledrag activity.  We use the 'weight' element
      // defined in Drupal's Form API.
      'weight' => array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#default_value' => $item->weight,
        '#delta' => 10,
        '#title_display' => 'invisible',
      ),
      'edit' => array(
//        '#type' => 'textfield',
//        '#default_value' => check_plain($item->description),
//        '#size' => 20,
//        '#maxlength' => 255,
        '#markup' => l(t('Edit'),'persona/edit/'.$item->id_people),  
      ),
      'delete' => array(
//        '#type' => 'textfield',
//        '#default_value' => check_plain($item->description),
//        '#size' => 20,
//        '#maxlength' => 255,
        '#markup' => l(t('Delete'),'persona/delete/'.$item->id_people),  
      ),
    );
  }
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save Changes'));
  dpm($form,'form');
  return $form;
}

function theme_table_dragg_form($variables) {
  $form = $variables['form'];

  // Initialize the variable which will store our table rows.
  $rows = array();

  // Iterate over each element in our $form['example_items'] array.
 // dpm(element_children($form['example_items']),'element_children');
  foreach (element_children($form['example_items']) as $id) {
    //drupal_set_message(t('id = @id', $id));
    // Before we add our 'weight' column to the row, we need to give the
    // element a custom class so that it can be identified in the
    // drupal_add_tabledrag call.
    //
    // This could also have been done during the form declaration by adding
    // '#attributes' => array('class' => 'example-item-weight'),
    // directy to the 'weight' element in tabledrag_example_simple_form().
    $form['example_items'][$id]['weight']['#attributes']['class'] = array('example-item-weight');

    // We are now ready to add each element of our $form data to the $rows
    // array, so that they end up as individual table cells when rendered
    // in the final table.  We run each element through the drupal_render()
    // function to generate the final html markup for that element.
    $rows[] = array(
      'data' => array(
        // Add our 'name' column.
        drupal_render($form['example_items'][$id]['name']),
        // Add our 'description' column.
        drupal_render($form['example_items'][$id]['email']),
        // Add our 'weight' column.
        drupal_render($form['example_items'][$id]['weight']),
        drupal_render($form['example_items'][$id]['edit']),
        drupal_render($form['example_items'][$id]['delete']),
      ),
      // To support the tabledrag behaviour, we need to assign each row of the
      // table a class attribute of 'draggable'. This will add the 'draggable'
      // class to the <tr> element for that row when the final table is
      // rendered.
      'class' => array('draggable'),
    );
  }

  // We now define the table header values.  Ensure that the 'header' count
  // matches the final column count for your table.
  $header = array(t('Name'), t('Email'), t('Weight'), t('Edit'), t('Delete'));

  // We also need to pass the drupal_add_tabledrag() function an id which will
  // be used to identify the <table> element containing our tabledrag form.
  // Because an element's 'id' should be unique on a page, make sure the value
  // you select is NOT the same as the form ID used in your form declaration.
  $table_id = 'example-items-table';

  // We can render our tabledrag table for output.
  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => $table_id),
  ));
  $output .= theme('pager');

  // And then render any remaining form elements (such as our submit button).
  $output .= drupal_render_children($form);

  // We now call the drupal_add_tabledrag() function in order to add the
  // tabledrag.js goodness onto our page.
  //
  // For a basic sortable table, we need to pass it:
  // - the $table_id of our <table> element,
  // - the $action to be performed on our form items ('order'),
  // - a string describing where $action should be applied ('siblings'),
  // - and the class of the element containing our 'weight' element.
  drupal_add_tabledrag($table_id, 'order', 'sibling', 'example-item-weight');
  
  return $output;
}

function my_search_submit($form, &$form_state){
 
  if(!empty ($form_state)){
    //if($form_state['values']['name'])

    $form_state['rebuild'] = TRUE;
  }
  
}

function table_dragg_form_submit($form, &$form_state) {
  // Because the form elements were keyed with the item ids from the database,
  // we can simply iterate through the submitted values.
  foreach ($form_state['values']['example_items'] as $id => $item) {
    db_query(
      "UPDATE {persona} SET weight = :weight WHERE id_people = :id",
      array(':weight' => $item['weight'], ':id' => $id)
    );
  }
}

