/**
 * Custom configuration for Storybook.
 */

// Using Production version of the asset building Webpack configuration to
// unify the building pipeline.
const { merge } = require('webpack-merge');
const webpack = require('webpack');
const path = require('path');
const custom = require('../webpack/webpack.prod');
const scssVariables = require('./importer.scss_variables');
const iconUtils = require('../components-combined/01-atoms/icon/icon.utils');
const backgroundUtils = require('../components-combined/01-atoms/background/background.utils');
const addonConfig = require('./addon-config').default();

const customPlugin = new webpack.DefinePlugin({
  SCSS_VARIABLES: JSON.stringify(scssVariables.getVariables()),
  ICONS: JSON.stringify({
    icons: iconUtils.getIcons(),
    packs: iconUtils.getIconPacks(),
  }),
  BACKGROUNDS: JSON.stringify(backgroundUtils.getBackgrounds()),
});

module.exports = {
  stories: [
    '../components-combined/**/*.stories.js',
  ],
  addons: addonConfig,
  webpackFinal: async (config) => {
    // Replace normal CSS import with stories CSS import, which already includes
    // normal CSS import. This is to allow to resolve variables and mixins in
    // stories CSS.
    custom.entry = custom.entry.map((value) => (value.indexOf('components_css.js') > -1 ? path.resolve(__dirname, 'components_css.stories.js') : value));

    // Remove theme-related entries as components should not have them.
    custom.entry = custom.entry.filter((p) => !/theme_/g.test(p));

    // Modify common configs to let Storybook take over.
    delete custom.output;
    delete custom.plugins;
    custom.plugins = [
      customPlugin,
    ];
    // Special case: override whatever loader is used to load styles with a
    // style-loader in oder to have styles injected during the runtime.
    custom.module.rules[1].use[0] = 'style-loader';

    return merge(config, custom);
  },
};
