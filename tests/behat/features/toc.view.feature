@civic @civic_toc
Feature: Tests the Table Of Contents

  Ensure that Table Of Contents works correctly.

  @api @javascript
  Scenario: Render TOC for the relevant data on the page if it is enabled or do not render it at all.
    Given civic_page content:
      | title                | status | field_c_n_show_toc |
      | [TEST] Page - toc    | 1      | 1                  |
      | [TEST] Page - no toc | 1      | 0                  |
    And "field_c_n_components" in "civic_page" "node" with "title" of "[TEST] Page - toc" has "civic_content" paragraph:
      | field_c_p_content:value  | <h2>Test heading 1</h2><p>Test content 1</p><h2>Test heading 2</h2><p>Test content 2</p> <h3>Test heading 3</h3><p>Test content 3</p> |
      | field_c_p_content:format | civic_rich_text                                                                                                                       |
    And "field_c_n_components" in "civic_page" "node" with "title" of "[TEST] Page - no toc" has "civic_content" paragraph:
      | field_c_p_content:value  | <h2>Test heading 1</h2><p>Test content 1</p><h2>Test heading 2</h2><p>Test content 2</p> <h3>Test heading 3</h3><p>Test content 3</p> |
      | field_c_p_content:format | civic_rich_text                                                                                                                       |

    When I visit civic_page "[TEST] Page - toc"
    Then I should see a ".table-of-contents-container" element
    And I should see a visible ".civic-table-of-contents__title" element
    And I should see the text "On this page"
    And I should see "Test heading 1" in the ".civic-table-of-contents__links" element
    And I should see "Test heading 2" in the ".civic-table-of-contents__links" element
    And I should not see "Test heading 3" in the ".civic-table-of-contents__links" element

    When I visit civic_page "[TEST] Page - no toc"
    Then I should not see a ".table-of-contents-container" element
    And I should not see a visible ".civic-table-of-contents__title" element