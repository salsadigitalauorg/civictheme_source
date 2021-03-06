@civictheme @paragraph @civictheme_callout
Feature: Tests the Callout

  Ensure that Callout paragraph exists and has the expected fields.

  @api
  Scenario: Paragraph type appears in the paragraph types page
    Given I am logged in as a user with the "Administrator" role
    When I go to "admin/structure/paragraphs_type"
    Then I should see the text "Callout" in the "civictheme_callout" row

  @api
  Scenario: Content paragraph exists with fields.
    Given I am logged in as a user with the "Administrator" role
    When I go to "admin/structure/paragraphs_type/civictheme_callout/fields"
    And I should see the text "field_c_p_title" in the "Title" row
    And I should see the text "field_c_p_theme" in the "Theme" row
    And I should see the text "field_c_p_summary" in the "Summary" row
    And I should see the text "field_c_p_links" in the "Links" row
    And I should see the text "field_c_p_space" in the "With space" row

  @api @javascript
  Scenario: Show relevant fields depending on the 'Content type' selected
    Given I am logged in as a user with the "Site Administrator" role
    When I visit "node/add/civictheme_page"
    And I fill in "Title" with "[TEST] Page fields"
    And I click on ".field-group-tabs-wrapper .horizontal-tab-button-2 a" element
    And I click on "div.field--name-field-c-n-components .paragraphs-add-wrapper .dropbutton-toggle button" element
    And I wait for AJAX to finish
    And I press the "field_c_n_components_civictheme_callout_add_more" button
    And I wait for AJAX to finish
    And should see an "select[name='field_c_n_components[0][subform][field_c_p_theme]']" element
    And should see an "select[name='field_c_n_components[0][subform][field_c_p_theme]'].required" element
    And should see an "select[name='field_c_n_components[0][subform][field_c_p_space]']" element
    And should see an "input[name='field_c_n_components[0][subform][field_c_p_title][0][value]']" element
    And should see an "input[name='field_c_n_components[0][subform][field_c_p_title][0][value]'].required" element
    And should see an "textarea[name='field_c_n_components[0][subform][field_c_p_summary][0][value]']" element
    And should see an "textarea[name='field_c_n_components[0][subform][field_c_p_summary][0][value]'].required" element
    And should see "The summary field may contain up to 100 characters. Any characters past the 100 character limit will not show for users." in the ".form-item-field-c-n-components-0-subform-field-c-p-summary-0-value" element
    And should see an "input[name='field_c_n_components[0][subform][field_c_p_links][0][uri]']" element
    And should see an "input[name='field_c_n_components[0][subform][field_c_p_links][0][uri]'].required" element
    And the option "Light" from select "Theme" is selected
