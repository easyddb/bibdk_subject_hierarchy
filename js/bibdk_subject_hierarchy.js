// Make sure Object.create is available in the browser (for our prototypal inheritance)
if ( typeof Object.create !== 'function' ) {
  Object.create = function(o) {
    function F() {};
    F.prototype = o;
    return new F();
  };
}


(function( $, window, document, undefined ) {

  Drupal.behaviors.bibdk_subject_hierarchy = {
    attach: function(context, settings) {
      $('.themes__item').each(function( index ) {
        new Drupal.ajax('.themes__item', $(this), {
          url: $(this).find('a').attr('href'),
          settings: {},
          event: 'click tap',
        });
      });
    }
  }

  var Themes = {
    init: function(options, elem) {
      var self = this;
      self.options = $.extend( {}, $.fn.themes.options, options );

      self.themes = elem;
      self.$themes = $(elem);

      self.armItems();
      self.armCloseButtons();

      $(window).bind('load resize', function() {
        self.sizeItems();
      });

    },

    armItems: function() {
      var self = this;
      self.$themes
        .find('.' + self.options.itemClass)
        .click(function(e) {
          e.preventDefault();
          self.toggleSublists($(this));
      });
    },

    armCloseButtons: function() {
      var self = this;
      self.$themes
        .find('.' + self.options.closeButtonClass)
        .click(function(e) {
          e.preventDefault();
          self.reset();
      });
    },

    toggleSublists: function ($target) {
      var self = this;

      // Target already has active class
      if ($target.hasClass(self.options.activeClass)) {
        self.reset();
      }
      // Sibling of target already has active class
      else if ($target.siblings().hasClass(self.options.activeClass)) {
        $target.siblings()
          .removeClass(self.options.activeClass);
        $target
          .addClass(self.options.activeClass);
      }
      // Either one if the items in other rows or none at all have active class
      else {
        self.reset();
        setTimeout(function() {
          $target.addClass(self.options.activeClass);
          $target
            .parent()
            .find('.' + self.options.sublistsWrapperClass)
            .addClass('foobar1')
            .fadeIn(200);
          // this below don't work??
          $target
            .closest('.' + self.options.rowClass)
            .find('.' + self.options.sublistsWrapperClass)
            .fadeIn(200);
        }, 200);
      }

    },

    reset: function () {
      var self = this;
      self.$themes
        .find('.' + self.options.itemClass)
        .removeClass(self.options.activeClass);
      self.$themes
        .find('.' + self.options.sublistsWrapperClass)
        .fadeOut(200);
    },


    sizeItems: function() {
      var self = this;
      self.$themesRows = self.$themes.find('.' + self.options.rowClass);

      // Loop through the rows and and act on the items in each row
      self.$themesRows.each(function() {
        var $items = $(this).find('.' + self.options.itemClass + ' > a');
        self.setHeighestHeight($items);
      });

    },

    setHeighestHeight: function($elements) {

      // We reset the height or it will influence the window resize event
      $elements.css({ 'height': '' });

      // Loop through each item and save the heighest height (hh)
      var hh = 0;
      $elements.each(function() {
        var itemHeight = $(this).height();
        if (itemHeight > hh) {
          hh = itemHeight;
        }
      });

      // Set height of each element to the height of the heighest
      $elements.css({ 'height': hh });
    }

  };
  // End 'var Themes'


  $.fn.themes = function(options) {
    return this.each(function() {
      var themes = Object.create(Themes);
      themes.init(options, this);
    });
  };

  $.fn.themes.options = {
    rowClass: 'themes__row',
    itemClass: 'themes__item',
    activeClass: 'themes__item--active',
    sublistsWrapperClass: 'themes__sublists__wrapper',
    closeButtonClass: 'themes__close-button'
  };

})( jQuery, window, document );
