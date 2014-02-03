(function ($) {
    Drupal.behaviors.subject_hierarchy_action = {
        attach: function (context) {
          console.log('attach');
            $('.themes__close-button', context).click(function (e) {
                e.preventDefault();
                $('.themes__item').removeClass('themes__item--active');
                $('.themes__sublists__wrapper').fadeOut('200');
            });

          /*
          $('#columns-wrapper', context).find('.themes__sublists__wrapper.row-1').css({
            'display': 'block',
            'background-color': 'red',
            'height':'100px'
          });

          $('#subjects-wrapper', context).find('.themes__sublists__wrapper.row-1').css({
            'display': 'block',
            'background-color': 'red',
            'height':'100px'
          });*/

          //TODO mmj if .themes__sublists__wrapper has content it should be displayed
          /*$('.themes__sublists__wrapper').each(function(index, element){
            if(element.innerHTML.length){
              $(element).css({
                'display':'block'
              });
            }
          });*/
        }
    };
}(jQuery));
