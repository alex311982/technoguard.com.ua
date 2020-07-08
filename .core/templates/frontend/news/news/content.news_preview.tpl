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
                <li><a href="/<!--{$lang}-->/about"><!--{$translations.menu_1}--></a></li>
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

<main class="heading case">
    <div class="container">
        <div class="heading__title">
            <h1><!--{$direction_title}--> </h1>
            <h2><!--{$item.news_title}--></h2>
        </div>
    </div>
    <div class="heading__image case">
        <img src="<!--{$item.imagePath}-->" alt="Retail Image">
    </div>
</main>

<section class="case-description">
    <div class="container small">
        <!--{$item.text_long}-->
        <div class="case-buttons">
            <a href="retail.html">
                <img class="right" src="/-images/new/assets/icons/ic_arrow.svg" alt="Arrow Icon">
                <!--{$translations.news_1}--> <!--{$direction_title}-->
            </a>
            <a class="right-button" href="/news.html?id=<!--{$next_news_id}-->">
                <!--{$translations.news_2}-->
                <img src="/-images/new/assets/icons/ic_arrow.svg" alt="Arrow Icon">
            </a>
        </div>
    </div>
</section>

<!--{if !empty($news)}-->
<section class="cases">
    <div class="container">
        <h2><!--{$translations.about_our_decision}--> <!--{$direction_title2}--></h2>
        <div class="cases__items">
            <!--{foreach from=$news item=news_item}-->
            <a href="/news.html?id=<!--{$news_item.id}-->" class="cases__item">
                <img src="<!--{$news_item.imagePath}-->" alt="adv 1">
                <p><!--{$news_item.news_title}--></p>
            </a>
            <!--{/foreach}-->
        </div>
    </div>
</section>
<!--{/if}-->

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
