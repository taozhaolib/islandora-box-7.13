<?php

/**
 * @file
 * Houses the administration for the islandora_oai module.
 */

/**
 * Creates the administration form for the islandora_oai module.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the form state.
 *
 * @return array
 *   A renderable array representing the islandora_oai administration form.
 */
function islandora_oai_admin_form($form, $form_state) {
  global $base_url;
  $form = array(
    '#tree' => TRUE,
  );
  $form['islandora_oai_configuration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configuration'),
    '#collapsible' => FALSE,
    '#collapsed' => TRUE,
  );
  $form['islandora_oai_configuration']['islandora_oai_repository_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Repository Name'),
    '#required' => TRUE,
    '#size' => '50',
    '#default_value' => variable_get('islandora_oai_repository_name', 'Repository'),
  );
  $form['islandora_oai_configuration']['islandora_oai_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to the Repository'),
    '#field_prefix' => $base_url . '/',
    '#required' => TRUE,
    '#size' => '50',
    '#default_value' => variable_get('islandora_oai_path', 'oai2'),
    '#description' => t('The path where the OAI-PMH service will respond, e.g. @base_url/oai2', array('@base_url' => $base_url)),
  );
  $form['islandora_oai_configuration']['islandora_oai_repository_identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('Repository unique identifier'),
    '#required' => TRUE,
    '#size' => '50',
    '#default_value' => variable_get('islandora_oai_repository_identifier', 'drupal-site.org'),
    '#description' => t('The identifier for this repository, e.g. oai:<strong>drupal-site.org</strong>:123.'),
  );
  $form['islandora_oai_configuration']['islandora_oai_admin_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Administrator Email'),
    '#size' => '50',
    '#default_value' => variable_get('islandora_oai_admin_email', 'N/A'),
  );
  $form['islandora_oai_configuration']['islandora_oai_max_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum Response Size'),
    '#size' => '50',
    '#default_value' => variable_get('islandora_oai_max_size', '20'),
    '#description' => t('The maximum number of records to issue per response. If the result set is larger than this number, a resumption token will be issued'),
  );
  $form['islandora_oai_configuration']['islandora_oai_expire_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Expiration Time'),
    '#size' => '50',
    '#default_value' => variable_get('islandora_oai_expire_time', '86400'),
    '#description' => t('The amount of time a resumption token will remain valid, in seconds. Defaults to one day (86400s).'),
  );
  $form['islandora_oai_configuration']['islandora_oai_date_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Solr Date Field'),
    '#size' => '50',
    '#default_value' => variable_get('islandora_oai_date_field', 'fgs_lastModifiedDate_dt'),
    '#description' => t('The Solr field containing the date to be used.'),
  );
  $form['islandora_oai_configuration']['islandora_oai_collection_field'] = array(
    '#type' => 'textarea',
    '#title' => t('Solr RELS-EXT Collection Field'),
    '#size' => '50',
    '#default_value' => variable_get('islandora_oai_collection_field', "RELS_EXT_isMemberOfCollection_uri_ms\nRELS_EXT_isMemberOf_uri_ms"),
    '#description' => t('The Solr fields used to determine what collection, if any, an object is a member of.'),
  );
  $form['islandora_oai_configuration']['islandora_oai_content_model_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Solr Content Model Field'),
    '#size' => '50',
    '#default_value' => variable_get('islandora_oai_content_model_field', 'RELS_EXT_hasModel_uri_ms'),
    '#description' => t("Field which RELS-EXT datastreams use to define an object's content model."),
  );
  $form['islandora_oai_configuration']['islandora_oai_exclude_content_models'] = array(
    '#type' => 'textarea',
    '#title' => t('Excluded Content Models'),
    '#size' => '50',
    '#default_value' => variable_get('islandora_oai_exclude_content_models', ''),
    '#description' => t('By default, all objects are visible to OAI metadata harvesters. This field allows you to exclude all objects with a certain content model, e.g "islandora:collectionCModel" to exclude all objects with the Islandora Core Collection content model.'),
  );

  $metadata_format_options = array();
  $metadata_formats = array();
  $results = db_query('SELECT * FROM {islandora_oai_metadata_formats} ORDER BY name');
  foreach ($results as $row) {
    $metadata_format_options[$row->name] = drupal_strtoupper($row->name);
    $metadata_formats[$row->name] = $row;
  }

  $form['islandora_oai_metadata'] = array(
    '#type' => 'fieldset',
    '#title' => t('Metadata Format'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['islandora_oai_metadata']['islandora_oai_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Transformations'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#weight' => 123,
  );
  $form['islandora_oai_metadata']['islandora_oai_metadata_format'] = array(
    '#type' => 'select',
    '#name' => 'islandora_oai_metadata_format',
    '#title' => t('Metadata Format'),
    '#options' => $metadata_format_options,
  );

  $oai_invoke_files = module_invoke_all('islandora_oai_get_xsl_files');
  $transform_options = array('default' => t('No transformation selected'));
  $transform_options = array_merge($transform_options, $oai_invoke_files);

  foreach ($metadata_formats as $format) {
    $default_transform = variable_get("islandora_oai_transform_file_$format->name", 'default');
    $form['islandora_oai_metadata'][$format->name]['islandora_oai_metadata_prefix'] = array(
      '#type' => 'item',
      '#title' => t('Metadata Prefix'),
      '#markup' => $format->metadata_prefix,
      '#description' => t('Default metadata prefix for the selected format.'),
      '#states' => array(
        'visible' => array(
          ':input[name="islandora_oai_metadata_format"]' => array('value' => $format->name),
        ),
      ),
    );
    $form['islandora_oai_metadata'][$format->name]['islandora_oai_metadata_namespace'] = array(
      '#type' => 'item',
      '#title' => t('Metadata Namespace'),
      '#markup' => $format->metadata_namespace,
      '#description' => t('Default metadata namespace for the selected format.'),
      '#states' => array(
        'visible' => array(
          ':input[name="islandora_oai_metadata_format"]' => array('value' => $format->name),
        ),
      ),
    );
    $form['islandora_oai_metadata'][$format->name]['islandora_oai_schema_location'] = array(
      '#type' => 'item',
      '#title' => t('Schema Location'),
      '#markup' => $format->oai2_schema,
      '#description' => t("Default URI for the selected metadata format's schema."),
      '#states' => array(
        'visible' => array(
          ':input[name="islandora_oai_metadata_format"]' => array('value' => $format->name),
        ),
      ),
    );
    $form['islandora_oai_metadata']['islandora_oai_options']["islandora_oai_metadata_datastream_id_$format->metadata_prefix"] = array(
      '#type' => 'textfield',
      '#size' => 30,
      '#title' => 'Metadata Datastream ID',
      '#default_value' => variable_get("islandora_oai_metadata_datastream_id_$format->metadata_prefix", 'DC'),
      '#description' => t('(Note that this is case-sensitive)'),
      '#states' => array(
        'visible' => array(
          ':input[name="islandora_oai_metadata_format"]' => array('value' => $format->name),
        ),
      ),
    );
    $form['islandora_oai_metadata']['islandora_oai_options']["islandora_oai_transform_file_$format->metadata_prefix"] = array(
      '#type' => 'select',
      '#title' => t('File to use for transforming @metadata_prefix', array('@metadata_prefix' => $format->metadata_prefix)),
      '#options' => $transform_options,
      '#default_value' => $default_transform,
      '#description' => t('XSL or XSLT file used to translate existing metadata to an appropriate OAI-PMH format.'),
      '#states' => array(
        'visible' => array(
          ':input[name="islandora_oai_metadata_format"]' => array('value' => $format->name),
        ),
      ),
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}

/**
 * Submit handler for the islandora_oai_admin form.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function islandora_oai_admin_form_submit($form, $form_state) {
  variable_set('islandora_oai_repository_name', $form_state['values']['islandora_oai_configuration']['islandora_oai_repository_name']);
  variable_set('islandora_oai_path', $form_state['values']['islandora_oai_configuration']['islandora_oai_path']);
  variable_set('islandora_oai_repository_identifier', $form_state['values']['islandora_oai_configuration']['islandora_oai_repository_identifier']);
  variable_set('islandora_oai_admin_email', $form_state['values']['islandora_oai_configuration']['islandora_oai_admin_email']);
  variable_set('islandora_oai_max_size', $form_state['values']['islandora_oai_configuration']['islandora_oai_max_size']);
  variable_set('islandora_oai_expire_time', $form_state['values']['islandora_oai_configuration']['islandora_oai_expire_time']);
  variable_set('islandora_oai_date_field', $form_state['values']['islandora_oai_configuration']['islandora_oai_date_field']);
  variable_set('islandora_oai_collection_field', $form_state['values']['islandora_oai_configuration']['islandora_oai_collection_field']);
  variable_set('islandora_oai_content_model_field', $form_state['values']['islandora_oai_configuration']['islandora_oai_content_model_field']);
  variable_set('islandora_oai_exclude_content_models', $form_state['values']['islandora_oai_configuration']['islandora_oai_exclude_content_models']);
  foreach ($form_state['values']['islandora_oai_metadata']['islandora_oai_options'] as $key => $value) {
    variable_set($key, $value);
  }
  // Because of the dynamic pathing of the OAI path we need to rebuild the
  // menus.
  menu_rebuild();
}

/**
 * Renders a file management form.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the Drupal form state.
 *
 * @return array
 *   An array representing the file management form.
 */
function islandora_oai_file_management_form($form, &$form_state) {
  // Grab all the user uploaded files.
  $oai_uploaded_files = array();
  $upload_path = 'public://islandora_oai_xsls';
  $uploaded_files = file_scan_directory($upload_path, '/.*\.xslt?$/');

  foreach ($uploaded_files as $up_file) {
    $oai_uploaded_files[$up_file->uri] = array(
      $up_file->filename,
      l(t('download'), file_create_url($up_file->uri)),
    );
  }
  ksort($oai_uploaded_files);
  $form['islandora_oai_files'] = array(
    '#type' => 'fieldset',
    '#title' => t('Files'),
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
  );
  $form['islandora_oai_files']['table'] = array(
    '#type' => 'tableselect',
    '#header' => array(
      t('File name'),
      t('Operations'),
    ),
    '#options' => $oai_uploaded_files,
    '#empty' => t('No uploaded files!'),
  );
  if (count($oai_uploaded_files)) {
    $form['islandora_oai_files']['remove_selected'] = array(
      '#type' => 'submit',
      '#value' => t('Delete selected'),
    );
  }
  $form['islandora_oai_files']['islandora_oai_upload'] = array(
    '#type' => 'fieldset',
    '#title' => t('Upload'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['islandora_oai_files']['islandora_oai_upload']['islandora_oai_upload_xsl'] = array(
    '#type' => 'file',
    '#title' => t('Upload an XSL or XSLT file to be used for transformations'),
    '#upload_location' => 'public://islandora_oai_xsls',
    '#upload_validators' => array(
      'file_validate_extensions' => array('xsl xslt'),
    ),
  );
  $form['islandora_oai_files']['islandora_oai_upload']['islandora_oai_upload_button'] = array(
    '#type' => 'submit',
    '#value' => t('Upload'),
    '#submit' => array('islandora_oai_upload_file'),
  );
  return $form;
}

/**
 * Validation for the islandora_oai_file_management form.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function islandora_oai_file_management_form_validate($form, $form_state) {
  if (end($form_state['triggering_element']['#parents']) == 'remove_selected') {
    $selected = FALSE;
    foreach ($form_state['values']['table'] as $value) {
      if ($value !== 0) {
        $selected = TRUE;
        break;
      }
    }
    if (!$selected) {
      form_set_error('table', t('Must select at least one entry to delete!'));
    }
  }
}
/**
 * Submit handler for the islandora_oai_file_management form.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function islandora_oai_file_management_form_submit($form, &$form_state) {
  $deleted_count = 0;
  $fid_or = db_or();
  foreach ($form_state['values']['table'] as $uri => $selected) {
    if ($selected !== 0) {
      $fid_or->condition('uri', $uri, '=');
    }
  }
  $fids = db_select('file_managed', 'f')
    ->fields('f', array('fid', 'uri'))
    ->condition($fid_or)
    ->execute()
    ->fetchAllAssoc('fid', PDO::FETCH_ASSOC);
  foreach ($fids as $fid) {
    file_delete(file_load($fid['fid']));
    $deleted_count++;
  }
  if ($deleted_count > 0) {
    drupal_set_message(format_plural($deleted_count,
      'Successfully deleted 1 file!',
      'Successfully deleted @count files!'
    ));
  }
}

/**
 * Submit handler for uploading a file.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function islandora_oai_upload_file($form, $form_state) {
  $file = file_save_upload('islandora_oai_upload_xsl', array('file_validate_extensions' => array('xsl xslt')), 'public://islandora_oai_xsls');
  if ($file) {
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
    drupal_set_message(t("The file @filename was uploaded successfully.",
      array(
        '@filename' => $file->filename,
      )
    ));
  }
}
