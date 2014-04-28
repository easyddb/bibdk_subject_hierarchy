(function ($) {
    Drupal.behaviors.subject_hierarchy_action = {
        attach: function (context) {
            $('.themes__close-button', context).click(function (e) {
                e.preventDefault();
                $('.themes__item').removeClass('themes__item--active');
                $('.themes__sublists__wrapper').fadeOut('200');
            });
            $('#bibdk-subject-hierachy-searchfield-form', context).ajaxComplete(function(event, xhr, settings) {
                if(event.target.id == 'bibdk-subject-hierachy-searchfield-form'){
                    console.log(xhr);
                }

            });
        }
    };
}(jQuery));
