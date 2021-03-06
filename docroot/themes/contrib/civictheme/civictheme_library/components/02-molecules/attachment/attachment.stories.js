import { boolean, radios, text } from '@storybook/addon-knobs';
import { getSlots } from '../../00-base/base.stories';
import CivicThemeAttachment from './attachment.twig';

export default {
  title: 'Molecules/Attachment',
};

export const Attachment = (knobTab) => {
  const generalKnobTab = typeof knobTab === 'string' ? knobTab : 'General';

  const date = new Date().toLocaleDateString('en-uk', {
    year: 'numeric',
    month: 'short',
    day: 'numeric',
  });

  const links = [
    {
      url: 'https://file-examples-com.github.io/uploads/2017/02/file-sample_100kB.doc',
      text: 'DOC type document',
      last_updated: date,
      icon: 'fileandfolders_wordfile',
    },
    {
      url: 'https://file-examples-com.github.io/uploads/2017/02/file-sample_100kB.docx',
      text: 'DOCX type document',
      last_updated: date,
      icon: 'fileandfolders_wordfile',
    },
    {
      url: 'https://file-examples-com.github.io/uploads/2017/10/file-sample_150kB.pdf',
      text: 'PDF type document',
      last_updated: date,
      icon: 'fileandfolders_pdffile',
    },
    {
      url: 'https://file-examples-com.github.io/uploads/2017/08/file_example_PPT_250kB.ppt',
      text: 'PPT type document',
      last_updated: date,
      icon: 'fileandfolders_clipboard_3',
    },
    {
      url: 'https://file-examples-com.github.io/uploads/2017/02/file_example_XLSX_10.xlsx',
      text: 'XLSX type document',
      last_updated: date,
      icon: 'fileandfolders_document',
    },
    {
      url: 'https://file-examples-com.github.io/uploads/2017/02/file_example_XLS_10.xls',
      text: 'XLS type document',
      last_updated: date,
      icon: 'fileandfolders_document',
    },
  ];

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
    title: text('Title', 'Event name which runs across two or three lines', generalKnobTab),
    summary: text('Summary', 'Card summary using body copy which can run across multiple lines. Recommend limiting this summary to three or four lines..', generalKnobTab),
    links: boolean('With links', true, generalKnobTab) ? links : null,
    vertical_space: radios(
      'Vertical space',
      {
        None: 'none',
        Top: 'top',
        Bottom: 'bottom',
        Both: 'both',
      },
      'none',
      generalKnobTab,
    ),
    modifier_class: text('Additional class', '', generalKnobTab),
  };

  return CivicThemeAttachment({
    ...generalKnobs,
    ...getSlots([
      'content_top',
      'content_bottom',
    ]),
  });
};
