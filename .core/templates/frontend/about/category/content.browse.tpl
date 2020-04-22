<div class="container smaller">
    <header class="header extra-padding">
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
                <li class="active"><a href="/<!--{$lang}-->/about"><!--{$translations.menu_1}--></a></li>
                <li><a href="/<!--{$lang}-->/contact"><!--{$translations.menu_2}--></a></li>
            </ul>
            <div class="cselect cselect--phone">
              <div data-select="ru" class="cselect__selected">Русский</div>
              <ul class="cselect__list">
                <li data-select="ua" class="cselect__item">Українською</li>
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
                <li class="active"><a href="/<!--{$lang}-->/about"><!--{$translations.menu_1}--></a></li>
                <li><a href="/<!--{$lang}-->/contact"><!--{$translations.menu_2}--></a></li>
            </ul>
            <div class="cselect">
              <div data-select="ru" class="cselect__selected">Русский</div>
              <ul class="cselect__list">
                <li data-select="ua" class="cselect__item">Українською</li>
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

<section class="contact-section">
    <div class="container smaller no-padding">
        <div class="contact-section__title">
            <h1>О компании <span>/</span></h1>
            <img class="blurred-line" src="/-images/new/assets/icons/blurred_line.svg" alt="blurred line">
        </div>
        <div class="contact-section__image">
            <img src="/-images/new/assets/images/about-image.png" alt="About image">
        </div>
    </div>
</section>

<section class="about-us">
    <div class="container smaller">
        <div class="about-us__block">
            <!--{$text}-->
        </div>
    </div>
</section>

<section class="contact">
    <div class="container">
        <div class="contact-form">
            <img src="/-images/new/assets/images/form-photo.png" alt="contact form photo">
            <div class="inner-form">
                <h3>Спросите нас</h3>
                <form action="./index.js">
                    <input required placeholder="Ваше имя" aria-placeholder="Ваше имя" type="text" name="name" id="name">
                    <input required placeholder="Email" aria-placeholder="Email" type="email" name="email" id="email">
                    <textarea placeholder="Опишите ваш запрос в двух словах" aria-placeholder="Опишите ваш запрос в двух словах"
                              name="about" id="about" rows="10"></textarea>
                    <button class="form-button" type="submit">Отправить запрос</button>
                </form>
                <div class="success-block">
                    <img src="/-images/new/assets/icons/ic_mark.svg" alt="Mark icon">
                    <p>Спасибо за ваше обращение.
                        Мы постараемся ответить быстро!</p>
                </div>
                <div class="error-block">
                    <img src="/-images/new/assets/icons/ic_mark.svg" alt="Mark icon">
                    <p>Спасибо за ваше обращение.
                        Ошибка отправки письма. Попробуйте еще.</p>
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
                    <li><a href="/contact">Контакты</a></li>
                </ul>
            </nav>
            <img class="blurred-line" src="/-images/new/assets/icons/blurred_line.svg" alt="blurred line">
            <img class="footer-logo" src="/-images/new/assets/images/bw_logo.svg" alt="Technoguard logo">
        </div>
    </div>
</footer>
