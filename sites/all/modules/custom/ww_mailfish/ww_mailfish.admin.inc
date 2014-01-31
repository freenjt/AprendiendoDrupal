<?php
function ww_mailfish_admin_settings_form() {
  $form = array();
  $form['ww_mailfish_types'] = array(
    '#title' => t('The content types to enable MailFish subscriptions
      for'),
    '#description' => t('On the specified node types, a MailFish
      subscription option will be available and can be enabled while that
      node is being edited.'),
    '#type' => 'checkboxes',
    '#options' => node_type_get_names(),
    '#default_value' => variable_get('ww_mailfish_types', array()),
  );

  return system_settings_form($form);
}