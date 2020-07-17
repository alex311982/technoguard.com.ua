$(document).ready(function () {
  const $slider = $(".slider");
  const $slideStatus = $(".current-slide-number");
  const $benefitLinks = $(".benefits__single a");
  const mainPageLinks = $(".tabs__caption li a");

  function showSlide(_, slick, current, _) {
    let i = (current ? current : 0) + 1;
    $slideStatus.html(
      `<div class="current-number">0${i}</div><div class="total-number">/0${slick.slideCount}`
    );
  }

  $(document).ready(function () {
    $slider.on("init", showSlide);
    $slider.slick({
      autoplay: false,
      arrows: true,
      prevArrow:
        "<button type='button' class='slick-prev'><i class='fa fa-angle-left' aria-hidden='true'></i></button>",
      nextArrow:
        "<button type='button' class='slick-next'><i class='fa fa-angle-right' aria-hidden='true'></i></button>",
      responsive: [
        {
          breakpoint: 769,
          settings: {
            arrows: false,
            dots: true,
            customPaging: function () {
              return "";
            },
          },
        },
      ],
    });

    $slider.on("init reInit afterChange", showSlide);

    $(".toggle-menu").click(function (e) {
      e.stopPropagation();
      toggleMenuState();
      if (window.innerWidth > 769 && window.innerWidth < 992) {
        $(window).on("click", closeMenuClickHandler);
      }
    });

    function closeMenuClickHandler(e) {
      if (
        !$(e.target).parents(".menu-mobile").length &&
        $(e.target)[0] !== $(".menu-mobile")[0]
      ) {
        console.log($(e.target).parents(".menu-mobile"));
        $(window).off("click", closeMenuClickHandler);
        toggleMenuState();
      }
    }

    function toggleMenuState() {
      $(".menu-mobile").toggleClass("active");
      $(".toggle-menu").toggleClass("active");
      $("body").toggleClass("overflow-hidden");
    }

    // $('ul.tabs__caption').on('click', 'li:not(.active)', function () {
    //   $(this)
    //     .addClass('active').siblings().removeClass('active')
    //     .closest('section.tabs').find('div.tabs__content').removeClass('active').eq($(this).index()).addClass('active');
    // });

    $(".inner-form form").submit(function (e) {
      e.preventDefault();

      $.post("/mail", {
        name: $('[name ="name"]').val(),
        email: $('[name ="email"]').val(),
        text: $('[name ="about"]').val(),
      })
        .done(function (data) {
          $(".inner-form form").css("display", "none");
          $(".success-block").css("display", "block");
        })
        .fail(function (error) {
          $(".inner-form form").css("display", "none");
          $(".error-block").css("display", "block");
        });
    });

    $(".benefits__single").hover(
      function () {
        $(this).addClass("special");
      },
      function () {
        $(this).removeClass("special");
      }
    );

    function scrollToAnchor(links) {
      links.each(function (_, v) {
        const $attribute = $(v).attr("data-scroll");
        const $div = $("div").find(`[data-scroll='${$attribute}']`);

        $(v).click(function () {
          $div.get(1).scrollIntoView();
        });
      });
    }

    scrollToAnchor($benefitLinks);

    let select = $(".cselect");
    let selectedEl;
    let arrowEl;
    if (select.length) {
      selectedEl = select.find(".cselect__selected");
      selectedEl.on("click", selectClickHandler);
      arrowEl = select.find(".cselect__arrow");
    }

    function selectClickHandler(e) {
      e.stopPropagation();
      toggleSelect(select);
      arrowEl.addClass("cselect__arrow--rotate");
      $(window).on("click", closeSelectClickHandler);
      selectedEl.off("click", selectClickHandler);
    }

    function closeSelectClickHandler(e) {
      e.stopPropagation();
      let clickedEl = $(e.target);
      if (clickedEl.hasClass("cselect__item")) {
        document.cookie =
          "lang=" +
          clickedEl.data("select") +
          "; path=/; expires=Tue, 19 Jan 2038 03:14:07 GMT";

        var parts = location.pathname.substring(1).split("/");

        if (parts[0].length == 2) {
          console.log("dddd");
          console.log(clickedEl.data("select"));
          parts[0] = clickedEl.data("select");
        }

        window.location.href =
          "http://technoguard.com.ua/" +
          parts.join("/") +
          "?" +
          window.location.search.replace("?", "");
      } else {
        toggleSelect(select);
        arrowEl.removeClass("cselect__arrow--rotate");
        $(window).off("click", closeSelectClickHandler);
        selectedEl.on("click", selectClickHandler);
      }
    }

    function toggleSelect(select) {
      select.find(".cselect__list").toggleClass("cselect__list--active");
    }
  });

  let navEl = $(".menu-mobile");
  let menuSecondaryButtonEl = navEl.find(".menu-mobile__secondary-button");
  menuSecondaryButtonEl.on("click", secondaryButtonClickHandler);
  function secondaryButtonClickHandler() {
    menuSecondaryButtonEl.off("click", secondaryButtonClickHandler);
    let tl = null;
    let animationEl = $(this).parent().parent();
    let thirdListEL = $(this).parent().next();
    let initialWrapEl = animationEl.parent();
    let buttonEl = animationEl.find(".menu-mobile__secondary-button");
    let buttonBackEl = animationEl.find(".menu-mobile__secondary-button-back");
    let mainListEl = navEl.find(".menu-mobile__main");
    let secondaryListEl = navEl.find(".menu-mobile__secondary");
    let emptyEl = $("<div class='buttonecondary-empty'></div>");
    let animationElInitialPosition = {
      top: animationEl.offset().top,
      left: animationEl.offset().left,
    };
    let secondaryListInitialPosition = {
      top: secondaryListEl.offset().top,
      left: secondaryListEl.offset().left,
    };

    replaceButtonWithEmptyEl();
    animateButtonMovement();

    function replaceButtonWithEmptyEl() {
      console.log(animationEl.outerWidth());
      emptyEl.css("height", animationEl.outerHeight() + "px");
      navEl.append(animationEl);
      initialWrapEl.append(emptyEl);
      animationEl.css({
        position: "fixed",
        left:
          window.innerWidth > 769
            ? 25 + "px"
            : animationElInitialPosition.left + "px",
        top: animationElInitialPosition.top + "px",
      });
    }

    function animateButtonMovement() {
      tl = gsap.timeline({
        onReverseComplete: () => {
          animationEl.css({
            position: "static",
            left: "auto",
            top: "auto",
          });
          emptyEl.replaceWith(animationEl);
          menuSecondaryButtonEl.on("click", secondaryButtonClickHandler);
        },
      });
      tl.add(gsap.to(mainListEl, { duration: 0.3, autoAlpha: 0 }));
      tl.call(() => {
        buttonEl.toggleClass("menu-mobile__secondary-button--active");
      }).to(animationEl, {
        duration: 0.2,
        css: { top: secondaryListInitialPosition.top + "px" },
      });
      tl.add(
        gsap.to(thirdListEL, {
          duration: 0.5,
          ease: "expo.out",
          css: { autoAlpha: 1, top: "100%" },
        }),
        "+=0.1"
      );
      tl.add(
        gsap.to(buttonBackEl, {
          duration: 0.5,
          ease: "expo.out",
          css: { autoAlpha: 1, left: "-15px" },
          onComplete: () => buttonBackEl.on("click", buttonBackClickHandler),
        }),
        "-=0.5"
      );
    }

    function buttonBackClickHandler() {
      buttonBackEl.off("click", buttonBackClickHandler);
      tl.reverse();
    }
  }
});
