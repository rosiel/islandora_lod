<?php

function islandora_authorities_chooser($build_id, $element, $string = ''){
  $source = islandora_lod_get_autocomplete_source($build_id, $element);
  switch ($source) {
    case "ntriples":
      islandora_lod_autocomplete_ntriples($build_id, $element, $string);
      break;
    default:
      module_load_include('inc', 'islandora_authority', 'includes/callback');
      islandora_authority_autocomplete($build_id, $element, $string);
      break;
  }
}

function islandora_lod_get_autocomplete_source($build_id, $element_hash){
  $form_state = form_state_defaults();
  $form = form_get_cache($build_id, $form_state);
  module_load_include('inc', 'objective_forms', 'Form');
  $obj_form = new Form($form, $form_state);
  $element = $obj_form->findElement($element_hash);
  $parent = $element->parent;
  if (isset($parent->controls['#user_data']['islandora_authority_source'])) {
    $source = $parent->controls['#user_data']['islandora_authority_source'];
    $temp = explode(':', $source);
    $source = $temp[0];
    return $source;
  }
  else {
    return NULL;
  }
}

function islandora_lod_autocomplete_ntriples($build_id, $element, $string){
  module_load_include('inc', 'islandora_authority', 'includes/callback');
  $fields = islandora_authority_get_fields($build_id, $element);

  $vocab_values = islandora_lod_ntriples_get_values('all', $string);
  $matches = array();
  foreach($vocab_values as $uri_raw => $value){
    $uri = trim($uri_raw, ' <>');
    $auth_uri = preg_replace('/\/[^\/]*$/', '', $uri);
    $lang = 'en';
    $option = array(
      'value' => $value,
      'valueURI' => $uri,
      'authorityURI' => $auth_uri,
      'lang' => $lang,
    );
    $matches[$uri] = islandora_authority_build_match($option, $fields);
  }
  drupal_json_output($matches);

}