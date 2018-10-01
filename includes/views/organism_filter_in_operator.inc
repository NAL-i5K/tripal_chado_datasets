<?php

/**
* Define my custom in_operator
*/
class organism_filter_in_operator extends views_handler_filter_in_operator {

// overwrite the get_value_options function.
function get_value_options() {
if (isset($this->value_options)) {
return;
}
//    $web_apollo_organism = variable_get('web_apollo_organism');
//    if (!empty($web_apollo_organism)) {
//      $organism_list = get_webapollo_organisms($web_apollo_organism);
//    }

$organism_list = chado_get_organism_select_options(FALSE);

$this->value_options = $organism_list;
}
}
