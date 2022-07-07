#!/usr/bin/env bash
##
# Provision content.
#
# shellcheck disable=SC2086

set -e
[ -n "${DREVOPS_DEBUG}" ] && set -x

# Path to the application.
APP="${APP:-/app}"

# Drush alias.
DRUSH_ALIAS="${DRUSH_ALIAS:-}"

# ------------------------------------------------------------------------------

# Use local or global Drush, giving priority to a local drush.
drush="$(if [ -f "${APP}/vendor/bin/drush" ]; then echo "${APP}/vendor/bin/drush"; else command -v drush; fi)"

echo "  > Provision default content."
$drush ${DRUSH_ALIAS} -y pm-enable civictheme_content

echo "  > Provision content."
$drush ${DRUSH_ALIAS} ev -v "require_once '/app/docroot/themes/contrib/civictheme/civictheme.provision.inc'; civictheme_provision_cli();"

echo "  > Enable helper module."
$drush ${DRUSH_ALIAS} -y pm-enable cs_core

echo "  > Generate test content."
GENERATED_CONTENT_CREATE=1 $drush ${DRUSH_ALIAS} -y pm-enable cs_generated_content

echo "  > Generate sitemap."
$drush ${DRUSH_ALIAS} simple-sitemap:generate
