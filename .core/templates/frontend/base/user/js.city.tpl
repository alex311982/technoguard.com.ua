<script type="text/javascript">
$(function(){
    var ekvCountryUkr = <!--{$country_ukr}-->;
    $("#idUserCountry").change(function(){
        var countrySelected = $(this).val();
        //--- ukr ---//
        if(countrySelected == ekvCountryUkr){
            $("#idUserCityIdDdl").show();
            $("#idUserCityText").hide();
        }else{
            $("#idUserCityIdDdl").hide();
            $("#idUserCityText").show();
        }
    }).change();
});
</script>