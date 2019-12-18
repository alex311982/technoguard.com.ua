$(document).ready(function() {
    //#------------------- top news carousel -------------------#//
    var newsSliderInHeader = $("#news_slider");
    newsSliderInHeader.carouFredSel({
        responsive	: false,
        auto 		: true,
        circular	: true,
        infinite	: false,
        pagination	: "#slider_pagi",
        scroll		: {
            fx			: "crossfade",
            duration	: 1000,
            speed		: 2500
        },
        items		: {
            visible		: 1,
            width		: 730
            , height: 264
        }
    });

    if(!EKV_LIVE_VERSION){
        newsSliderInHeader.trigger("stop");
    }

});

$(window).load(function(){
    //#------------------- bottom presenters carousel  -------------------#//
    var presCrouselPagHandler = $("#presenterCrousel");
    presCrouselPagHandler.carouFredSel({
        responsive	: false,
        auto 		: true,
        circular	: true,
        infinite	: false,
        pagination	: "#presenterCrouselPagination",
        height: '800px',
        scroll		: {
            items       : 4,
            fx			: "crossfade",
            duration	: 1000,
            speed		: 2500
        },
        items		: {
            visible		: 4,
            width		: 186
        }
    });

    if(!EKV_LIVE_VERSION){
        presCrouselPagHandler.trigger("stop");
    }
});