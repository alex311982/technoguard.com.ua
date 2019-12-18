$(document).ready(function () {
  const $slider = $('.slider');
  const $slideStatus = $('.current-slide-number');
  const $benefitLinks = $('.benefits__single a');

  function showSlide(_, slick, current, _) {
    let i = (current ? current : 0) + 1;
    $slideStatus.html(`<div class="current-number">0${i}</div><div class="total-number">/0${slick.slideCount}`);
  }

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

  $('ul.tabs__caption').on('click', 'li:not(.active)', function () {
    $(this)
        .addClass('active').siblings().removeClass('active')
        .closest('section.tabs').find('div.tabs__content').removeClass('active').eq($(this).index()).addClass('active');
  });

  $('.inner-form form').submit(function (e) {
    e.preventDefault();
    $('.inner-form form').css('display', 'none');
    $('.success-block').css('display', 'block');
  })

  $benefitLinks.each(function (_, v) {
    const $attribute = $(v).attr('data-scroll');
    const $div = $('div').find(`[data-scroll='${$attribute}']`);

    $(v).click(function () {
      $div.get(1).scrollIntoView()
    });
  })
});
