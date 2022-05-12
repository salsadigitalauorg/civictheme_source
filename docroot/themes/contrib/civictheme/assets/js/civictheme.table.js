/**
 * Adds data-table attributes to CivicTheme table columns.
 */
Drupal.behaviors.civictheme_table = {
  // eslint-disable-next-line no-unused-vars
  attach: function attach(context, settings) {
    // eslint-disable-next-line no-undef
    const $table = jQuery('.civictheme-basic-content table', context).once('civicthemeTable');
    if ($table.length === 0) {
      return;
    }

    // Add titles to cells via thead.
    const addTheadColumnTitles = ($table) => {
      // Determine whether column titles can be added via thead.
      const $theadRows = $table.find('thead tr');
      const $tbodyRows = $table.find('tbody tr');
      if (!($theadRows.length && $tbodyRows.length)) {
        return;
      }
      const $theadRow = $theadRows.first();
      const $theadCells = $theadRow.find('th, td');
      $tbodyRows.each(function (rowIndex) {
        const $row = jQuery(this);
        $tbodyRowCells = $row.find('th, td').each(function (cellIndex) {
          // Only add titles to cells that do not have titles.
          if (jQuery(this).attr('data-title') === undefined) {
            jQuery(this).attr('data-title', jQuery($theadCells[cellIndex]).text());
          }
        });
      });
    };

    // Add titles to cells in rows with row-scoped th cells.
    const addRowScopedTitles = ($table) => {};

    // Add titles to cells in rows with col-scoped th cells.
    const addColScopedTitles = ($table) => {};

    const addTitles = ($table) => {
      addTheadColumnTitles($table);
      // todo: addRowScopedTitles();
      // todo: addColScopedTitles();
    };

    $table.each(function (index) {
      addTitles(jQuery(this));
    });
  },
};
