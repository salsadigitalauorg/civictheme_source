import {
  boolean, number, radios,
} from '@storybook/addon-knobs';
import CivicBreadcrumb from './breadcrumb.twig';
import { randomLinks } from '../../00-base/base.stories';

export default {
  title: 'Organisms/Breadcrumb',
};

export const Breadcrumb = (knobTab) => {
  const generalKnobTab = typeof knobTab === 'string' ? knobTab : 'General';

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
    active_is_link: boolean('Active is link', false, generalKnobTab),
    links: randomLinks(number(
      'Number of links',
      7,
      {
        range: true,
        min: 0,
        max: 10,
        step: 1,
      },
      generalKnobTab,
    )),
  };

  return CivicBreadcrumb({
    ...generalKnobs,
  });
};
