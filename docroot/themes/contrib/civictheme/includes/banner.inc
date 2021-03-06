<?php

/**
 * @file
 * Banner component alterations.
 */

use Drupal\Core\Cache\Cache;
use Drupal\node\NodeInterface;

/**
 * Defines Small banner style.
 */
define('CIVICTHEME_BANNER_TYPE_DEFAULT', 'default');

/**
 * Defines Large banner style.
 */
define('CIVICTHEME_BANNER_TYPE_LARGE', 'large');

/**
 * Defines Large banner style.
 */
define('CIVICTHEME_BANNER_DEFAULT_THEME', 'light');

/**
 * Pre-process for Programmatic Block - Banner block.
 *
 * @SuppressWarnings(PHPMD.StaticAccess)
 */
function _civictheme_preprocess_block__civictheme_banner(&$variables) {
  if ($variables['base_plugin_id'] != 'block_content') {
    return;
  }

  $entity = $variables['elements']['content']['#block_content'];
  if ($entity->bundle() != 'civictheme_banner' || ($entity->hasField('field_c_b_type') && $entity->field_c_b_type->isEmpty())) {
    return;
  }

  $variables['content'] = NULL;

  // Cache tags.
  /** @var \Drupal\node\Entity\Node $node */
  $node = \Drupal::routeMatch()->getParameter('node');
  if ($node instanceof NodeInterface) {
    $variables['#cache']['tags'] = Cache::mergeTags($entity->getCacheTags(), $node->getCacheTags());
  }
  $variables['#cache']['contexts'][] = 'url.path';
  $variables['#cache']['contexts'][] = 'url.query_args';

  $variables['type'] = CIVICTHEME_BANNER_TYPE_DEFAULT;

  $variables['theme'] = civictheme_get_theme_value($entity) ?? CIVICTHEME_BANNER_DEFAULT_THEME;

  if (civictheme_field_has_value($entity, 'field_c_b_background_image')) {
    $variables['background_image'] = civictheme_media_get_url_from_field($entity, 'field_c_b_background_image');
  }

  _civictheme_preprocess_civictheme_banner_title($variables);
  _civictheme_preprocess_civictheme_banner_node($variables);
  _civictheme_preprocess_civictheme_banner_breadcrumb($variables);

  if ($variables['type'] == CIVICTHEME_BANNER_TYPE_LARGE) {
    $variables['is_decorative'] = TRUE;
  }

  // Explicitly defined variable to allow additional preprocessing.
  $variables['content_top1'] = NULL;
  $variables['content_top2'] = NULL;
  $variables['content_top3'] = NULL;
  $variables['content_bottom'] = NULL;
}

/**
 * Pre-process banner title.
 *
 * @SuppressWarnings(PHPMD.StaticAccess)
 */
function _civictheme_preprocess_civictheme_banner_title(&$variables) {
  // Banner title.
  $request = \Drupal::request();
  $route_match = \Drupal::routeMatch();
  $page_title = \Drupal::service('title_resolver')->getTitle($request, $route_match->getRouteObject());

  /** @var \Drupal\node\Entity\Node $node */
  $node = \Drupal::routeMatch()->getParameter('node');
  $node_revision = \Drupal::routeMatch()->getParameter('node_revision');

  if ($node_revision !== NULL) {
    $node = $node_revision instanceof NodeInterface ? $node_revision : node_revision_load($node_revision);
  }

  if (!is_null($node)) {
    $page_title = civictheme_get_field_value($node, 'field_c_n_banner_title') ?? $page_title;
  }

  $variables['title'] = $page_title ?? '';
}

/**
 * Pre-process banner breadcrumb.
 *
 * @SuppressWarnings(PHPMD.StaticAccess)
 * @SuppressWarnings(PHPMD.CyclomaticComplexity)
 */
function _civictheme_preprocess_civictheme_banner_breadcrumb(&$variables) {
  // Disable breadcrumb on homepage.
  if (\Drupal::service('path.matcher')->isFrontPage()) {
    return;
  }

  /** @var \Drupal\node\Entity\Node $node */
  $node = \Drupal::routeMatch()->getParameter('node');
  $node_revision = \Drupal::routeMatch()->getParameter('node_revision');

  if ($node_revision !== NULL) {
    $node = $node_revision instanceof NodeInterface ? $node_revision : node_revision_load($node_revision);
  }

  // Breadcrumb visibility.
  $hide_breadcrumb = FALSE;
  if (!is_null($node)) {
    $hide_breadcrumb = civictheme_get_field_value($node, 'field_c_n_banner_hide_breadcrumb') ?? FALSE;
  }

  if (!$hide_breadcrumb) {
    /** @var \Drupal\Core\Routing\RouteMatch $route_match */
    $route_match = \Drupal::getContainer()->get('current_route_match');
    /** @var \Drupal\Core\Breadcrumb\Breadcrumb|null $breadcrumb */
    $breadcrumb = \Drupal::service('breadcrumb')->build($route_match);
    if ($breadcrumb) {
      $variables['breadcrumb'] = $breadcrumb->toRenderable();
      if (isset($variables['theme'])) {
        // Changing to CivicTheme theme to pass other variables to template.
        $variables['breadcrumb']['#theme'] = 'civictheme_breadcrumb';
        $variables['breadcrumb']['#display_theme'] = $variables['theme'];
      }
    }
  }
}

/**
 * Pre-process banner for all nodes.
 *
 * @SuppressWarnings(PHPMD.StaticAccess)
 */
function _civictheme_preprocess_civictheme_banner_node(&$variables) {
  /** @var \Drupal\node\Entity\Node $node */
  $node = \Drupal::routeMatch()->getParameter('node');
  $node_revision = \Drupal::routeMatch()->getParameter('node_revision');

  if ($node_revision !== NULL) {
    $node = $node_revision instanceof NodeInterface ? $node_revision : node_revision_load($node_revision);
  }

  if (is_null($node)) {
    return;
  }

  $variables['type'] = civictheme_get_field_value($node, 'field_c_n_banner_type');

  $variables['theme'] = civictheme_get_theme_value($node) ?? CIVICTHEME_BANNER_DEFAULT_THEME;

  $is_front = \Drupal::service('path.matcher')->isFrontPage();
  $variables['is_front'] = $is_front;

  _civictheme_preprocess_civictheme_banner_last_updated($variables, $node);
  _civictheme_preprocess_civictheme_banner_background_image($variables, $node);
  _civictheme_preprocess_civictheme_banner_featured_image($variables, $node);
  _civictheme_preprocess_civictheme_banner_components($variables, $node);
}

/**
 * Preprocess banner last updated date.
 */
function _civictheme_preprocess_civictheme_banner_last_updated(&$variables, $node) {
  $last_updated = civictheme_get_field_value($node, 'field_c_n_show_last_updated');
  if ($last_updated) {
    $variables['content_middle'][] = [
      '#theme' => 'civictheme_basic_content',
      '#component_theme' => $variables['theme'] ?? NULL,
      '#content' => '<p class="civictheme-banner__last_updated">Last updated: ' . \Drupal::service('date.formatter')->format($node->getChangedTime(), 'civictheme_short_date') . '<p>',
    ];
  }
}

/**
 * Preprocess banner background image.
 */
function _civictheme_preprocess_civictheme_banner_background_image(&$variables, $node) {
  $background_image = civictheme_get_field_value($node, 'field_c_n_banner_background', TRUE);
  if ($background_image !== NULL) {
    $variables['background_image'] = civictheme_media_get_url($background_image);
  }
}

/**
 * Preprocess banner featured image.
 */
function _civictheme_preprocess_civictheme_banner_featured_image(&$variables, $node) {
  $featured_image = civictheme_get_field_value($node, 'field_c_n_banner_featured_image', TRUE);
  if ($featured_image !== NULL) {
    $image = civictheme_get_field_value($featured_image, 'field_c_m_image', TRUE);
    $variables['featured_image']['src'] = civictheme_media_get_url($featured_image);
    $variables['featured_image']['alt'] = $image !== NULL ? $image->getValue()['alt'] : '';
  }
}

/**
 * Preprocess banner components.
 */
function _civictheme_preprocess_civictheme_banner_components(&$variables, $node) {
  $banner_components = civictheme_get_field_value($node, 'field_c_n_banner_components');
  if ($banner_components) {
    $render_controller = \Drupal::entityTypeManager()->getViewBuilder('paragraph');
    $variables['content'][] = $render_controller->viewMultiple($banner_components, 'default');
  }

  // Banner content below components.
  $banner_below_components = civictheme_get_field_value($node, 'field_c_n_banner_components_bott');
  if ($banner_below_components) {
    $render_controller = \Drupal::entityTypeManager()->getViewBuilder('paragraph');
    $variables['content_below'][] = $render_controller->viewMultiple($banner_below_components, 'default');
  }
}
