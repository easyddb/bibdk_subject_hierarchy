(function ($) {
    Drupal.behaviors.subject_hierarchy_action = {
        attach: function (context) {
            $('.themes__close-button', context).click(function (e) {
                e.preventDefault();
                $('.themes__item').removeClass('themes__item--active');
                $('.themes__sublists__wrapper').fadeOut('200');
            });
        }
    };
    Drupal.ajax.prototype.commands.afterAjaxSubmit = function(ajax, response, status) {
        $(response.selector).replaceWith(response.value);
        $('#bibdk-subject-hierarchy-content').hide();
        $(response.selector).hide();
        $(response.selector).fadeIn('200');
        $('.close', response.selector).click(function (event) {
            event.preventDefault();
            $(response.selector).hide();
            $('#bibdk-subject-hierarchy-content').fadeIn('200');
        });
    }

}(jQuery));
