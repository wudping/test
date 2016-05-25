



function btn_1_click()
{
    //alert('test');
    window.cpp.JS_btn_1('btn1 click');
}




function btn_2_click()
{
    //alert('test');
    window.cpp.JS_btn_2('btn2 click');
}





function btn_3_click()
{
    //alert("test");
    var str = document.getElementById('text_edit').value;
    //alert(str);
    window.cpp.JS_btn_3(str);
}




function ChangeText(text) {
    $('#text').html(text);
}


function ChangeText_2(text) {
    $('#text').html(text + '    abccc');
}