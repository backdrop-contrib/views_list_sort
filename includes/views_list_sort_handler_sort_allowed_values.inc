<?php

/**
 * Allowed values sort handler.
 */
class views_list_sort_handler_sort_allowed_values extends views_handler_sort {

  /**
   * Affect the query.
   */
  function query() {
    // Skip if disabled.
    if (!$this->options['allowed_values']) {
      return;
    }

    // Get database connection.
    $target = !empty($this->query->options) ? 'replica' : 'default';
    $key = isset($this->view->base_database) ? $this->view->base_database : 'default';
    $connection = Database::getConnection($target, $key);

    // Build sanitized list of keys from field's allowed values.
    $keys = [];
    $allowed_values = list_allowed_values(field_info_field($this->definition['field_name']));

    // Key sort:
    // sort($allowed_values);

    // Natural sort, case insensitive, on allowed values:
    natcasesort($allowed_values);

    foreach ($allowed_values as $key => $value) {
      $keys[] = $connection->quote($key);
    }

    // Add order by expression.
    $this->ensure_my_table();
    $expression = "FIELD($this->table_alias.$this->real_field, " . implode(', ', $keys) . ")";
    $this->query->add_orderby(NULL, NULL, $this->options['order'], $expression);
  }

  /**
   * Options definition.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['allowed_values'] = ['default' => 0];
    return $options;
  }

  /**
   * Options form.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['allowed_values'] = [
      '#type' => 'checkbox',
      '#title' => t('Sort by allowed values (labels)'),
      '#default_value' => $this->options['allowed_values'],
    ];
  }

}
