@civictheme @content_type @civictheme_event
Feature: Fields on Page content type

  Ensure that Review fields have been setup correctly.

  Background:
    Given civictheme_event content:
      | title                   | status    |
      | [TEST] Test Event title | published |

  @api
  Scenario: Event content type exists with fields.
    Given I am logged in as a user with the "Administrator" role
    When I go to "admin/structure/types/manage/civictheme_event/fields"
    And I should see the text "field_c_n_attachments" in the "Attachments" row
    And I should see the text "field_c_n_body" in the "Body" row
    And I should see the text "field_c_n_show_toc" in the "Show Table of Contents" row
    And I should see the text "field_c_n_date" in the "Event date" row
    And I should see the text "field_c_n_image" in the "Featured image" row
    And I should see the text "field_c_n_location" in the "Location" row
    And I should see the text "field_c_n_more_information" in the "More information" row
    And I should see the text "field_c_n_tagline" in the "Tagline" row
    And I should see the text "field_c_n_thumbnail" in the "Thumbnail" row

  @api
  Scenario: Event content type page has the relevant fields
    Given I am logged in as a user with the "Site Administrator" role
    When I visit "node/add/civictheme_event"
    Then the response status code should be 200

    And I see field "Title"
    And should see an "input#edit-title-0-value" element
    And should see an "input#edit-title-0-value.required" element
    And should not see an "input#edit-title-0-value[disabled]" element

    And I see field "Tagline"
    And I see field "field_c_n_body[0][value]"
    And I see field "field_c_n_show_toc[value]"
    And I should see an "input[name='field_c_n_location_civictheme_map_add_more']" element
    And I should see an "input[name='field_c_n_attachments_civictheme_attachment_add_more']" element
    And I should see an "input[name='field_c_n_thumbnail-media-library-open-button']" element
    And I should see an "input[name='field_c_n_image-media-library-open-button']" element
    And I should see an "input[name='field_c_n_more_information_civictheme_promo_add_more']" element
    And I see field "field_c_n_date[0][value][date]"
    And I see field "field_c_n_date[0][end_value][date]"
