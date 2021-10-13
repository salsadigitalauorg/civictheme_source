import { radios, text } from '@storybook/addon-knobs';
import CivicSearch from './search.twig';

export default {
  title: 'Molecule/Search',
  parameters: {
    layout: 'centered',
  },
};

export const Search = () => {
  const generalKnobTab = 'General';

  const generalKnobs = {
    theme: radios(
      'Theme',
      {
        Light: 'light',
        Dark: 'dark',
      },
      'light',
      generalKnobTab,
    ),
    placeholder: text('Placeholder', 'Enter keywords or phrase', generalKnobTab),
    button_text: text('Button text', 'Search', generalKnobTab),
    description: text('Description', 'Search by keyword', generalKnobTab),
  };

  const html = CivicSearch({
    ...generalKnobs,
  });

  return `<div class="story-wrapper-size--large">${html}</div>`;
};
