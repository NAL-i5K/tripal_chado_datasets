<?php

/**
 * @file
 * Install, update, and uninstall functions for the Foo Bar module.
 */


function tripal_chado_datasets_views_data() {
  $data['ds_organism_assembly']['table']['group'] = t('ds_organism_assembly');

  $data['ds_organism_assembly']['table']['base'] = [
    'field' => 'oa_id',
    'title' => t('ds_organism_assembly'),
    'help' => t('ds_organism_assembly contains assembly dataset for organisms.'),
    'weight' => -10,
  ];

  $data['ds_organism_assembly']['table']['join'] = [
    'node' => [
      'left_field' => 'oa_id',
      'field' => 'oa_id',
    ],
  ];

  $data['ds_organism_assembly']['oa_id'] = [
    'title' => t('oa_id'),
    'help' => t('Organism assembly id.'),
    /*'relationship' => array(
      'base' => 'node',
      'base field' => 'oa_id', // The name of the field on the joined table.
      // 'field' => 'nid' -- see hook_views_data_alter(); not needed here.
      'handler' => 'views_handler_relationship', 
      'label' => t('organism assembly'),
    ),	*/
    'field' => [
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'views_handler_filter_numeric',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];

  $data['ds_organism_assembly']['name'] = [
    'title' => t('co-ordinator name'),
    'help' => t('Just a name field.'),
    'field' => [
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
    'filter' => [
      'handler' => 'views_handler_filter_string',
    ],
    'argument' => [
      'handler' => 'views_handler_argument_string',
    ],
  ];

  $data['ds_organism_assembly']['email'] = [
    'title' => t('Email'),
    'help' => t('Just a Email field.'),
    'field' => [
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'views_handler_filter_string',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];

  $data['ds_organism_assembly']['taxid'] = [
    'title' => t('Taxid'),
    'help' => t('Just an taxid field.'),
    'field' => [
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'views_handler_filter_numeric',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];

  $data['ds_organism_assembly']['organism'] = [
    'title' => t('Organism'),
    'help' => t('Organism field.'),
    'field' => [
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'organism_filter_in_operator',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];

  $data['ds_organism_assembly']['common_name'] = [
    'title' => t('common_name'),
    'help' => t('common_name field.'),
    'field' => [
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'views_handler_filter_string',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];

  $data['ds_organism_assembly']['timestamp_field'] = [
    'title' => t('Timestamp'),
    'help' => t('Just a timestamp field.'),
    'field' => [
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ],
    'sort' => [
      'handler' => 'views_handler_sort_date',
    ],
    'filter' => [
      'handler' => 'views_handler_filter_date',
    ],
  ];

  $data['ds_gene_prediction']['table']['group'] = t('ds_gene_prediction');

  $data['ds_gene_prediction']['table']['base'] = [
    'field' => 'pid',
    'title' => t('ds_gene_prediction'),
    'help' => t('ds_gene_prediction contains gene prediction data for organisms.'),
    'weight' => -10,
  ];

  $data['ds_gene_prediction']['table']['join'] = [
    'node' => [
      'left_field' => 'pid',
      'field' => 'pid',
    ],
  ];

  $data['ds_gene_prediction']['pid'] = [
    'title' => t('pid'),
    'help' => t('gene prediction id.'),
    'field' => [
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'views_handler_filter_numeric',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];

  $data['ds_gene_prediction']['name'] = [
    'title' => t('co-ordinator name'),
    'help' => t('Just a name field.'),
    'field' => [
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
    'filter' => [
      'handler' => 'views_handler_filter_string',
    ],
    'argument' => [
      'handler' => 'views_handler_argument_string',
    ],
  ];

  $data['ds_gene_prediction']['email'] = [
    'title' => t('Email'),
    'help' => t('Just a Email field.'),
    'field' => [
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'views_handler_filter_string',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];


  $data['ds_gene_prediction']['organism'] = [
    'title' => t('Organism'),
    'help' => t('Organism field.'),
    'field' => [
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'organism_filter_in_operator',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];

  return $data;
}

