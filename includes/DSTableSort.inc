<?php

class DsTableSort extends TableSort {

/**
* Compose a URL query parameter array for table sorting links.
*
* @return
*   A URL query parameter array that consists of all components of the
*   current page request except for those pertaining to table sorting.
*/
public function tablesort_get_query_parameters() {
return drupal_get_query_parameters($_GET, ['q', 'sort', 'order', 'nid']);
}
}