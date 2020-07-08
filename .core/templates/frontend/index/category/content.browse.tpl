<div class="container smaller">
    <header class="header">
        <a href=".">
            <img src="/-images/new/assets/images/technoguard_logo.svg" alt="Technoguard Logo">
        </a>

        <button aria-label="Toggle menu" class="toggle-menu">
            <span></span>
            <span></span>
            <span></span>
        </button>

        <nav role="navigation" class="menu-mobile">
            <img src="/-images/new/assets/images/technoguard_logo.svg" alt="Technoguard Logo">
            <ul>
                <li><a href="/<!--{$lang}-->/about"><!--{$translations.menu_1}--></a></li>
                <li><a href="/<!--{$lang}-->/contact"><!--{$translations.menu_2}--></a></li>
            </ul>
            <div class="cselect cselect--phone">
              <div data-select="<!--{$selectedLangCode}-->" class="cselect__selected"><!--{$selectedLang}--></div>
                <ul class="cselect__list">
                    <li data-select="<!--{$itemLangCode}-->" class="cselect__item"><!--{$itemLang}--></li>
                </ul>
              <svg class="cselect__arrow" xmlns="http://www.w3.org/2000/svg" width="8.828" height="5.828" viewBox="0 0 8.828 5.828">
                <g id="Group_30" data-name="Group 30" transform="translate(1.414 1.414)" opacity="0.5">
                  <line id="Line_12" data-name="Line 12" x2="3" y2="3" fill="none" stroke="#707070" stroke-linecap="round" stroke-width="2"/>
                  <line id="Line_13" data-name="Line 13" x1="3" y2="3" transform="translate(3)" fill="none" stroke="#707070" stroke-linecap="round" stroke-width="2"/>
                </g>
              </svg>
            </div>
        </nav>

        <nav role="navigation" class="menu">
            <ul>
                <li><a href="/<!--{$lang}-->/about""><!--{$translations.menu_1}--></a></li>
                <li><a href="/<!--{$lang}-->/contact"><!--{$translations.menu_2}--></a></li>
            </ul>
            <div class="cselect">
              <div data-select="<!--{$selectedLangCode}-->" class="cselect__selected"><!--{$selectedLang}--></div>
              <ul class="cselect__list">
                <li data-select="<!--{$itemLangCode}-->" class="cselect__item"><!--{$itemLang}--></li>
              </ul>
              <svg class="cselect__arrow" xmlns="http://www.w3.org/2000/svg" width="8.828" height="5.828" viewBox="0 0 8.828 5.828">
                <g id="Group_30" data-name="Group 30" transform="translate(1.414 1.414)" opacity="0.5">
                  <line id="Line_12" data-name="Line 12" x2="3" y2="3" fill="none" stroke="#707070" stroke-linecap="round" stroke-width="2"/>
                  <line id="Line_13" data-name="Line 13" x1="3" y2="3" transform="translate(3)" fill="none" stroke="#707070" stroke-linecap="round" stroke-width="2"/>
                </g>
              </svg>
            </div>
        </nav>
    </header>
</div>

<section class="main">
    <!--{if !empty($banners)}-->
        <div class="slider">
            <!--{foreach from=$banners item=banner}-->
                <div class="slide">
                    <div class="slide__image">
                        <img src="<!--{$banner.imagePath}-->" alt="Slider image">
                        <a href="<!--{$banner.url}-->">
                            <!--{$translations.indexpage_banner_button_getacknowledgment}-->
                        </a>
                    </div>
                    <div class="slide__right">
                        <div>
                            <span><!--{$banner.subtitle}--></span>
                            <h1><!--{$banner.title}--></h1>
                        </div>
                    </div>
                </div>
            <!--{/foreach}-->
        </div>
    <!--{/if}-->
    <div class="current-slide-number"></div>
</section>

<div class="container">
    <section class="tabs">

        <ul class="tabs__caption">
            <li class="active"><a href="#retail" data-scroll="retail"><!--{$translations.indexpage_retail}--></a></li>
            <li><a href="#agroprom" data-scroll="agroprom"><!--{$translations.indexpage_agroprom}--></a></li>
            <li><a href="#rfid" data-scroll="rfid"><!--{$translations.indexpage_rfid}--></a></li>
        </ul>

        <div class="tabs__content active">
            <!--{foreach from=$indexblocks item=indexblock}-->
                <div class="tab__content">
                    <div id="retail" class="tab__main left-position">
                        <h2><!--{$indexblock.title}--></h2>
                        <p><!--{$indexblock.text}--></p>
                        <a href="/<!--{$lang}-->/direction.html?type=<!--{$indexblock.direction_type}-->">
                            <img src="/-images/new/assets/icons/ic_arrow.svg" alt="arrow icon">
                            <!--{$indexblock.button_title}-->
                        </a>
                    </div>
                    <img class="tab__image" src="<!--{$indexblock.imagePath}-->" alt="tab 1">
                </div>
            <!--{/foreach}-->
        </div>
    </section>
    <section class="tabs-mobile">
        <div data-scroll="retail">
            <a href="/direction.html?type=retail" class="tabs-mobile-content">
                <h2><!--{$translations.indexpage_retail}--></h2>
                <img class="tab__image" src="/-images/new/assets/images/tab-1.png" alt="tab 1">
                <p><!--{$translations.indexpage_retail_text}--></p>
            </a>
        </div>
        <div data-scroll="agroprom">
            <a href="/direction.html?type=agroprom" class="tabs-mobile-content">
                <h2><!--{$translations.indexpage_agroprom}--></h2>
                <img class="tab__image" src="/-images/new/assets/images/tab-2.png" alt="tab 1">
                <p><!--{$translations.indexpage_agroprom_text}--></p>
            </a>
        </div>
        <div data-scroll="rfid">
            <a href="/direction.html?type=RFID" class="tabs-mobile-content">
                <h2><!--{$translations.indexpage_rfid}--></h2>
                <img class="tab__image" src="/-images/new/assets/images/tab-1.png" alt="tab 1">
                <p><!--{$translations.indexpage_rfid_text}--></p>
            </a>
        </div>
    </section>
</div>

<section class="contact">
    <div class="container">
        <div class="contact-form">
            <img src="/-images/new/assets/images/form-photo.png" alt="contact form photo">
            <div class="inner-form">
                <h3><!--{$translations.global_ask_us}--></h3>
                <form action="./index.js">
                    <input required placeholder="<!--{$translations.global_ask_us_your_name}-->" aria-placeholder="<!--{$translations.global_ask_us_your_name}-->" type="text" name="name" id="name">
                    <input required placeholder="Email" aria-placeholder="Email" type="email" name="email" id="email">
                    <textarea placeholder="<!--{$translations.global_ask_us_question}-->"
                              aria-placeholder="<!--{$translations.global_ask_in_2_words}-->" name="about" id="about" rows="10"></textarea>
                    <button class="form-button" type="submit"><!--{$translations.global_ask_us_button_send}--></button>
                </form>
                <div class="success-block">
                    <img src="/-images/new/assets/icons/ic_mark.svg" alt="Mark icon">
                    <p><!--{$translations.global_ask_us_thanks}--></p>
                </div>
            </div>
        </div>
    </div>
</section>

<footer class="footer">
    <div class="container">
        <div class="footer__inner">
            <nav class="footer__menu">
                <ul>
                    <li><a href="https://www.facebook.com/TechnoguardUA/">Facebook</a></li>
                    <li><a href="/<!--{$lang}-->/contact"><!--{$translations.global_footer_contacts}--></a></li>
                </ul>
            </nav>
            <img class="blurred-line" src="/-images/new/assets/icons/blurred_line.svg" alt="blurred line">
            <img class="footer-logo" src="/-images/new/assets/images/bw_logo.svg" alt="Technoguard logo">
        </div>
    </div>
</footer>
