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
                <li><a href="/about">О компании</a></li>
                <li><a href="/contact">Контакты</a></li>
            </ul>
        </nav>

        <nav role="navigation" class="menu">
            <ul>
                <li><a href="/about">О компании</a></li>
                <li><a href="/contact">Контакты</a></li>
            </ul>
        </nav>
    </header>
</div>

<main class="heading case">
    <div class="container">
        <div class="heading__title">
            <h1><!--{$direction_title}--> <span>/</span></h1>
            <h2><!--{$item.title}--></h2>
        </div>
    </div>
    <div class="heading__image case">
        <img src="<!--{$item.imagePath}-->" alt="Retail Image">
    </div>
</main>

<section class="case-description">
    <div class="container small">
        <!--{$item.long_text}-->
        <div class="case-buttons">
            <a href="direction.html?type=<!--{$item.direction_type}-->">
                <img class="right" src="/-images/new/assets/icons/ic_arrow.svg" alt="Arrow Icon">
                В раздел <!--{$direction_title}-->
            </a>
            <!--{if $next_news_id}-->
                <a class="right-button" href="/decision.html?id=<!--{$next_news_id}-->">
                    Следующая
                    <img src="/-images/new/assets/icons/ic_arrow.svg" alt="Arrow Icon">
                </a>
            <!--{/if}-->
        </div>
    </div>
</section>

<!--{if !empty($news)}-->
<section class="cases">
    <div class="container">
        <h2>Читайте о нашем опыте решений для <!--{$direction_title2}--></h2>
        <div class="cases__items">
            <!--{foreach from=$news item=news_item}-->
            <a href="/decision.html?id=<!--{$news_item.id}-->" class="cases__item">
                <img src="<!--{$news_item.imagePath}-->" alt="adv 1">
                <p><!--{$news_item.title}--></p>
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