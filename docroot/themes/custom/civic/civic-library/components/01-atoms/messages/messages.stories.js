import {boolean, radios, text} from "@storybook/addon-knobs";
import CivicMessages from './messages.twig';

/**
 * Storybook Definition.
 */
export default {
  title: 'Atom/Messages'
};

export const Messages = () => CivicMessages({
  theme: radios(
    'Theme',
    {
      'Light': 'light',
      'Dark': 'dark',
    },
    'light',
  ),
  type: radios(
    'Type',
    {
      'Info': 'info',
      'Error': 'error',
      'Warning': 'warning',
      'Success': 'success',
    },
    'info',
  ),
  title: text('Message title', 'The information on this page is currently being updated.'),
  description: text('Message summary', 'Please contact CASA for the latest updates and information. Filium morte multavit si sine causa, nollem me tamen laudandis. Learn more.'),
});
