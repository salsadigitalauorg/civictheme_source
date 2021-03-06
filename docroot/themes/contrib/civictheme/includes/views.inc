<?php

/**
 * @file
 * Views component alterations.
 */

use Drupal\node\Entity\Node;
use Drupal\Core\Render\Element;
use Drupal\views\ViewExecutable;
use Drupal\Core\Cache\Cache;

/**
 * Defines default view theme.
 */
define('CIVICTHEME_VIEWS_DEFAULT_THEME', 'light');

/**
 * Page pre-process for views.
 */
function _civictheme_preprocess_page__views(&$variables) {
  $route_defaults = \Drupal::routeMatch()->getRouteObject()->getDefaults();
  if (isset($route_defaults['view_id'])) {
    // View pages should not be contained.
    $variables['page']['content_contained'] = FALSE;
  }
}

/**
 * Implements template_preprocess_views_mini_pager().
 */
function civictheme_preprocess_views_mini_pager(&$variables) {
  if (isset($variables['items']['current'])) {
    $variables['items']['pages'] = [
      $variables['items']['current'] => [
        'href' => '#',
      ],
    ];
  }
}

/**
 * Implements hook_preprocess_HOOK().
 */
function civictheme_preprocess_views_exposed_form(&$variables) {
  $variables['filter_title'] = 'Filter search results by:';
  $variables['large_filter'] = $variables['form']['#large_filter'] ?? FALSE;
  $is_view_component = isset($variables['attributes']['civictheme_listing']);
  if ($is_view_component) {
    $variables += $variables['attributes']['civictheme_listing'];
    unset($variables['attributes']['civictheme_listing']);
    $variables['theme'] = $variables['theme'] ?? 'light';
    _civictheme_propagate_component_theme_settings($variables['form'], $variables['theme']);
  }
  $variables['theme'] = $variables['theme'] ?? 'light';
}

/**
 * Propagate component theme setting to nested form elements.
 *
 * @SuppressWarnings(PHPMD.StaticAccess)
 */
function _civictheme_propagate_component_theme_settings(&$elements, $theme) {
  $form_elements = Element::children($elements);
  foreach ($form_elements as $form_element) {
    $elements[$form_element]['#attributes']['#civictheme_theme'] = $theme;
    $children = Element::children($elements[$form_element]);
    if (!empty($children)) {
      _civictheme_propagate_component_theme_settings($elements[$form_element], $theme);
    }
  }
}

/**
 * Implements hook_preprocess_HOOK().
 *
 * @SuppressWarnings(PHPMD.CyclomaticComplexity)
 * @SuppressWarnings(PHPMD.NPathComplexity)
 */
function civictheme_preprocess_views_view(&$variables) {
  $view = &$variables['view'];
  $is_page_load = isset($view->civictheme_listing_entity);
  if ($is_page_load) {
    _civictheme_listing_preprocess_view($view->civictheme_listing_entity, $view);
  }
  else {
    $listing_paragraph = _civictheme_get_listing_paragraph_from_route();
    _civictheme_listing_preprocess_view($listing_paragraph, $view);
  }

  $route_parameters = \Drupal::routeMatch()->getParameters();
  $is_view_page = $route_parameters->get('view_id') !== NULL;
  $is_view_component = isset($variables['view']->civictheme_listing);

  // Adds component theme settings to view / listing template.
  if ($is_view_component) {
    $variables += $variables['view']->civictheme_listing;
    // Override already set title.
    $variables['title'] = $variables['view']->civictheme_listing['title'] ?? '';
    if (!empty($variables['exposed'])) {
      $variables['exposed']['#attributes']['civictheme_listing'] = $variables['view']->civictheme_listing;
    }
    if (!empty($variables['pager'])) {
      $variables['pager']['#attributes']['#civictheme_theme'] = $variables['view']->civictheme_listing['theme'] ?? 'light';
    }
  }
  // Header is displayed within card container.
  if ($view->getStyle()->getBaseId() === 'grid' && isset($view->header['result'])) {
    unset($variables['header']);
  }
  // Adding template required classes in for civictheme component.
  $variables['theme'] = $variables['theme'] ?? 'light';
  $variables['with_background'] = $variables['with_background'] ?? $is_view_page;

  if (!empty($variables['attributes']['class'])) {
    $variables['modifier_class'] = $variables['attributes']['class'];
    unset($variables['attributes']['class']);
  }
  else {
    $variables['modifier_class'] = [];
  }
  $variables['modifier_class'] = array_merge($variables['modifier_class'], [
    !empty($variables['dom_id']) ? ' js-view-dom-id-' . $variables['dom_id'] : '',
  ]);
  $variables['modifier_class'] = implode(' ', $variables['modifier_class']);
  if (isset($view->civictheme_listing['show_filters']) && $view->civictheme_listing['show_filters'] === FALSE) {
    $variables['exposed'] = NULL;
  }
}

/**
 * Adds preprocessing data to view to provide to listing child components.
 */
function _civictheme_listing_preprocess_view($paragraph, ViewExecutable &$view) {
  $settings = [];
  if ($paragraph !== NULL) {
    $link = civictheme_get_field_value($paragraph, 'field_c_p_link', TRUE);
    $link_data = $link ? [
      'text' => $link->get('title')->getString(),
      'is_external' => $link->isExternal(),
      'is_new_window' => $link->isExternal(),
      'url' => $link->getUrl()->toString(),
    ] : [];

    $read_more = civictheme_get_field_value($paragraph, 'field_c_p_read_more', TRUE);
    $read_more_data = $read_more ? [
      'text' => $read_more->get('title')->getString(),
      'is_external' => $read_more->isExternal(),
      'is_new_window' => $read_more->isExternal(),
      'url' => $read_more->getUrl()->toString(),
    ] : [];

    $settings = [
      'theme' => civictheme_get_theme_value($paragraph) ?? CIVICTHEME_VIEWS_DEFAULT_THEME,
      'card_theme' => civictheme_field_has_value($paragraph, 'field_c_p_card_theme') ? $paragraph->get('field_c_p_card_theme')->getString() : CIVICTHEME_VIEWS_DEFAULT_THEME,
      'link' => $link_data,
      'title' => civictheme_get_field_value($paragraph, 'field_c_p_title') ?? '',
      'with_background' => civictheme_get_field_value($paragraph, 'field_c_p_background') ?? FALSE,
      'vertical_space' => civictheme_get_field_value($paragraph, 'field_c_p_space') ?? '',
      'show_filters' => civictheme_get_field_value($paragraph, 'field_c_p_show_filters') ?? TRUE,
      'read_more' => $read_more_data,
      'cache_tags' => $paragraph->getCacheTags(),
    ];
  }

  // Call modules/theme that implement the hook, and let them change the view.
  \Drupal::moduleHandler()->alter('civictheme_listing_preprocess_view', $settings, $view);
  \Drupal::service('theme.manager')->alter('civictheme_listing_preprocess_view', $settings, $view);

  $view->civictheme_listing = $settings;

}

/**
 * Implements hook_preprocess_HOOK().
 *
 * @SuppressWarnings(PHPMD.StaticAccess)
 */
function civictheme_preprocess_views_view_grid(&$variables) {
  $view = $variables['view'] ?? NULL;
  $variables['modifier_class'] = 'civictheme-card-container--listing';
  // On Grid view title should be added to card container.
  if ($view && $view->getStyle()->getBaseId() === 'grid' && isset($view->header['result'])) {
    $variables['title'] = $view->header['result']->render();
  }
  // Adds component theme settings to view / listing template.
  $is_view_component = isset($variables['view']->civictheme_listing);
  if ($is_view_component) {
    $variables += $variables['view']->civictheme_listing;
  }
  if (isset($variables['card_theme'])) {
    foreach (array_keys($variables['rows']) as $id) {
      $variables['rows'][$id]['#node']->civictheme_theme = $variables['card_theme'];
      $variables['rows'][$id]['#cache']['contexts'][] = 'url.path';
      $variables['rows'][$id]['#cache']['contexts'][] = 'url.query_args';
      if (!empty($variables['view']->civictheme_listing['cache_tags'])) {
        $variables['rows'][$id]['#cache']['tags'] = Cache::mergeTags($variables['view']->civictheme_listing['cache_tags'], $view->getCacheTags());
      }
    }
  }
}

/**
 * Gets the Listing paragraph from route.
 *
 * @return \Drupal\paragraphs\Entity\Paragraph|null
 *   First Listing paragraph from the node in route or NULL if no listing
 *   paragraphs are present.
 *
 * @SuppressWarnings(PHPMD.StaticAccess)
 */
function _civictheme_get_listing_paragraph_from_route() {
  $node = \Drupal::routeMatch()->getParameter('node');

  // Handling an ajax request: try and get from query parameters.
  if (!$node) {
    $nid = \Drupal::request()->get('nid');
    if (is_numeric($nid)) {
      $node = Node::load($nid);
    }
  }

  if ($node && $node->bundle() == 'civictheme_page') {
    $components = civictheme_get_field_referenced_entities($node, 'field_c_n_components');
    foreach ($components as $component) {
      if ($component->getType() == 'civictheme_listing') {
        return $component;
      }
    }
  }

  return NULL;
}
