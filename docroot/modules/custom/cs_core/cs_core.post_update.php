<?php

/**
 * @file
 * Post update hooks for core.
 */

use Drupal\redirect\Entity\Redirect;
use Drupal\user\Entity\User;

/**
 * Creates administrator users.
 */
function cs_core_post_update_provision_users() {
  $emails = [
    'akhil.bhandari@salsadigital.com.au',
    'alan.rako@salsadigital.com.au',
    'alan@salsadigital.com.au',
    'alex.skrypnyk@salsadigital.com.au',
    'govind@salsadigital.com.au',
    'john.cloys@salsadigital.com.au',
    'joshua.fernandes@salsadigital.com.au',
    'nick.georgiou@salsadigital.com.au',
    'richard.gaunt@salsadigital.com.au',
    'sonam.chaturvedi@salsadigital.com.au',
  ];

  foreach ($emails as $email) {
    $user = User::create();
    $user->setUsername($email);
    $user->setEmail($email);
    $user->addRole('civictheme_site_administrator');
    $user->activate();
    $user->enforceIsNew();
    $user->save();
  }
}

/**
 * Creates storybook redirects.
 */
function cs_core_post_update_provision_storybook_redirects() {
  $map = [
    [
      'src' => '/storybook',
      'dst' => '/themes/contrib/civictheme/civictheme_library/storybook-static/index.html',
    ],
    [
      'src' => '/storybook-drupal',
      'dst' => '/themes/contrib/civictheme/storybook-static/index.html',
    ], [
      'src' => '/storybook-drupal-demo',
      'dst' => '/themes/custom/civictheme_demo/storybook-static/index.html',
    ],
  ];

  foreach ($map as $item) {
    $redirect = Redirect::create();
    $redirect->setSource($item['src']);
    $redirect->setRedirect($item['dst']);
    $redirect->setStatusCode(301);
    $redirect->save();
  }
}
