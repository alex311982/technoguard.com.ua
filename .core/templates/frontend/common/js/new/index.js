$(document).ready(function () {
  const $slider = $('.slider');
  const $slideStatus = $('.current-slide-number');
  const $benefitLinks = $('.benefits__single a');
  const mainPageLinks = $('.tabs__caption li a')

  function showSlide(_, slick, current, _) {
    let i = (current ? current : 0) + 1;
    $slideStatus.html(`<div class="current-number">0${i}</div><div class="total-number">/0${slick.slideCount}`);
  }

  $(document).ready(function () {
    $slider.on('init', showSlide);
    $slider.slick({
      autoplay: false,
      arrows: true,
      prevArrow: "<button type='button' class='slick-prev'><i class='fa fa-angle-left' aria-hidden='true'></i></button>",
      nextArrow: "<button type='button' class='slick-next'><i class='fa fa-angle-right' aria-hidden='true'></i></button>",
      responsive: [
        {
          breakpoint: 769,
          settings: {
            arrows: false,
            dots: true,
            customPaging: function () {
              return '';
            }
          }
        }
      ]
    });

    $slider.on('init reInit afterChange', showSlide);

    $('.toggle-menu').click(function () {
      $('.menu-mobile').toggleClass('active');
      $(this).toggleClass('active');
    })

    // $('ul.tabs__caption').on('click', 'li:not(.active)', function () {
    //   $(this)
    //     .addClass('active').siblings().removeClass('active')
    //     .closest('section.tabs').find('div.tabs__content').removeClass('active').eq($(this).index()).addClass('active');
    // });

    $('.inner-form form').submit(function (e) {
      e.preventDefault();

      $.post('/mail', {name : $('[name ="name"]').val()  , email :$('[name ="email"]').val(), text :$('[name ="about"]').val()  })
          .done(function (data) {
            $('.inner-form form').css('display', 'none');
            $('.success-block').css('display', 'block');
          })
          .fail(function (error) {
            $('.inner-form form').css('display', 'none');
            $('.error-block').css('display', 'block');
          });
    })

    $('.benefits__single').hover(function () {
      $(this).addClass('special');
    }, function () {
      $(this).removeClass('special');
    });

    function scrollToAnchor(links) {
      links.each(function (_, v) {
        const $attribute = $(v).attr('data-scroll');
        const $div = $('div').find(`[data-scroll='${$attribute}']`);

        $(v).click(function () {
          $div.get(1).scrollIntoView()
        });
      })
    }

    scrollToAnchor($benefitLinks)
  });
});
