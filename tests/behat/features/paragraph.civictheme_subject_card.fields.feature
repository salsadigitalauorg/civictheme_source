@civictheme @paragraph @civictheme_subject_card
Feature: Tests the Subject card paragraph

  Ensure that Subject card paragraph exists and has the expected fields.

  @api
  Scenario: Paragraph type appears in the paragraph types page
    Given I am logged in as a user with the "Administrator" role
    When I go to "admin/structure/paragraphs_type"
    Then I should see the text "Subject card" in the "civictheme_subject_card" row

  @api
  Scenario: Subject card paragraph exists with fields.
    Given I am logged in as a user with the "Administrator" role
    When I go to "admin/structure/paragraphs_type/civictheme_subject_card/fields"
    And I should see the text "field_c_p_theme" in the "Theme" row
    And I should see the text "field_c_p_title" in the "Title" row
    And I should see the text "field_c_p_image" in the "Image" row
    And I should see the text "field_c_p_link" in the "Link" row

  @api
  Scenario: Subject card paragraph field_c_p_cards fields settings.
    Given I am logged in as a user with the "Administrator" role
    When I go to "admin/structure/paragraphs_type/civictheme_card_container/fields/paragraph.civictheme_card_container.field_c_p_cards"
    And the "Label" field should contain "Cards"
    Then the option "Default" from select "Reference method" is selected
    Then the "Include the selected below" checkbox should be checked
    And the "Subject card" checkbox should be checked

  @api @javascript
  Scenario: Show relevant fields depending on the 'Content type' selected
    Given I am logged in as a user with the "Site Administrator" role
    When I visit "node/add/civictheme_page"
    And I fill in "Title" with "[TEST] Page fields"
    And I click on ".field-group-tabs-wrapper .horizontal-tab-button-2 a" element
    And I click on "div.field--name-field-c-n-components .paragraphs-add-wrapper .dropbutton-toggle button" element
    And I wait 1 second
    And I press the "field_c_n_components_civictheme_card_container_add_more" button
    And I wait for AJAX to finish
    And the option "Light" from select "Theme" is selected
    And I should see an "div.js-form-item-field-c-n-components-0-subform-field-c-p-title-0-value" element
    And I should see an "div.js-form-item-field-c-n-components-0-subform-field-c-p-header-link-0-uri" element
    And I should see an "div.js-form-item-field-c-n-components-0-subform-field-c-p-column-count select.required" element
    And I should see an "select[name='field_c_n_components[0][subform][field_c_p_column_count]']" element
    And I should see an "input[name='field_c_n_components[0][subform][field_c_p_fill_width][value]']" element
    And I click on "div.field--name-field-c-p-cards .paragraphs-add-wrapper .dropbutton-toggle button" element
    And I wait 1 second
    And I press the "field_c_n_components_0_subform_field_c_p_cards_civictheme_subject_card_add_more" button
    And I wait for AJAX to finish
    Then select "field_c_n_components[0][subform][field_c_p_cards][0][subform][field_c_p_theme]" should have an option "light"
    And I should see an "input[name='field_c_n_components[0][subform][field_c_p_cards][0][subform][field_c_p_title][0][value]']" element
    And I should see an "input[name='field_c_n_components[0][subform][field_c_p_cards][0][subform][field_c_p_title][0][value]'].required" element
    And I should see an "input[name='field_c_p_image-media-library-open-button-field_c_n_components-0-subform-field_c_p_cards-0-subform']" element
    And I should see an "input[name='field_c_n_components[0][subform][field_c_p_cards][0][subform][field_c_p_link][0][uri]']" element
    And I should see an "input[name='field_c_n_components[0][subform][field_c_p_cards][0][subform][field_c_p_link][0][title]']" element
    And I should see an "div.field--name-field-c-p-image.field--widget-media-library-widget .js-media-library-widget.required" element
