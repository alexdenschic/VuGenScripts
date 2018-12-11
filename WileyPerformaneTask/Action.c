Action()
{
	int i;
	int j;
	int valueCount;
	int elemLength;
	int elemIdx;
	
	web_set_max_html_param_len("3000");

	web_reg_save_param_attrib(
		"ParamName=secret",
		"TagName=input",
		"Extract=value",
		"Name=__secret",
		"Type=hidden",
		SEARCH_FILTERS,
		"IgnoreRedirections=No",
		LAST);

	web_reg_save_param_attrib(
		"ParamName=timestamp",
		"TagName=input",
		"Extract=value",
		"Name=__timestamp",
		"Type=hidden",
		SEARCH_FILTERS,
		"IgnoreRedirections=No",
		LAST);

	lr_start_transaction("LoginPage");
	web_url("quiz_loginpage",
		"URL={Host}/quiz", 
		"Resource=0", 
		"RecContentType=text/html", 
		"Referer=", 
		"Snapshot=t85.inf", 
		"Mode=HTML", 
		LAST);
	lr_end_transaction("LoginPage", LR_AUTO);

	lr_think_time(10);
	
	lr_start_transaction("LoginRequest");
	web_custom_request("quiz_login", 
		"URL={Host}/quiz", 
		"Method=POST", 
		"Resource=0", 
		"RecContentType=application/json", 
		"Referer={Host}/quiz", 
		"Snapshot=t50.inf", 
		"Mode=HTTP", 
		"EncType=application/x-www-form-urlencoded; charset=UTF-8", 
		"Body=name={Username}&password={Password}&__timestamp={timestamp}&__secret={secret}", 
		LAST);
	lr_end_transaction("LoginRequest", LR_AUTO);

	// parametr contains number of question
	lr_save_string("1", "quizNumber");

// Cycle of question's requests
while(strcmp(lr_eval_string("{quizNumber}"), "submit") != 0) {
	web_reg_save_param_attrib(
		"ParamName=secret",
		"TagName=input",
		"Extract=value",
		"Name=__secret",
		"Type=hidden",
		SEARCH_FILTERS,
		"IgnoreRedirections=No",
		LAST);

	web_reg_save_param_attrib(
		"ParamName=timestamp",
		"TagName=input",
		"Extract=value",
		"Name=__timestamp",
		"Type=hidden",
		SEARCH_FILTERS,
		"IgnoreRedirections=No",
		LAST);

//	<div class="form-group"><textarea name="a5pi94vweutaumv" class="form-control" rows="3"></textarea></div>
	web_reg_save_param_ex(
		"ParamName=textarea", 
		"LB=textarea name=\"", 
		"RB=\" class=",
		"Ordinal=All",
		"Notfound=warning",
		LAST);

//<div class="form-group"><input type="text" class="form-control" name="ausgkdn9zc"></div>
	web_reg_save_param_ex(
		"ParamName=inputText", 
		"LB=input type=\"text\" class=\"form-control\" name=\"", 
		"RB=\"></div>",
		"Ordinal=All",
		"Notfound=warning",
		LAST);

//<div class="form-group"><select name="6adrh2kf11d" class="form-control"><option value=""></option><option value="8537">8537</option>
//<option value="3">6adrh2kf11d</option><option value="90334">90334</option></select></div>
	web_reg_save_param_ex(
		"ParamName=selectName", 
		"LB=select name=\"", 
		"RB=\" class=",
		"Ordinal=All",
		"Notfound=warning",
		LAST);

	web_reg_save_param_ex(
		"ParamName=selectValue", 
		"LB=</option><option value=\"", 
		"RB=\">",
		"Ordinal=All",
		"Notfound=warning",
		LAST);

// <div class="form-group"><div class="radio"><label><input name="e4tqr75cilljm" type="radio" value="49787"> 49787</label></div>
// <div class="radio"><label><input name="e4tqr75cilljm" type="radio" value="2"> e4tqr75cilljm</label></div>
// <div class="radio"><label><input name="e4tqr75cilljm" type="radio" value="91508"> 91508</label></div></div>\n
	web_reg_save_param_ex(
		"ParamName=radioName", 
		"LB=<div class=\"form-group\"><div class=\"radio\"><label><input name=\"", 
		"RB=\" type=\"radio",
		"Ordinal=All",
		"Notfound=warning",
		LAST);

	web_reg_save_param_ex(
		"ParamName=radioValue", 
		"LB=radio\" value=\"", 
		"RB=\">",
		"Ordinal=All",
		"Notfound=warning",
		LAST);


//<div class="form-group"><div class="checkbox"><label><input name="664221" type="checkbox"> 664221</label></div><div class="checkbox">
//<label><input name="631817" type="checkbox"> 631817</label></div><div class="checkbox"><label><input name="w22r1oo81d2n" type="checkbox"> w22r1oo81d2n</label>
	web_reg_save_param_ex(
		"ParamName=checkbox", 
		"LB=input name=\"", 
		"RB=\" type=\"checkbox\"",
		"Ordinal=All",
		"Notfound=warning",
		LAST);

	lr_think_time(10);
	
	lr_start_transaction("Question");
	web_url("quiz", 
		"URL={Host}/quiz/question/{quizNumber}", 
		"Resource=0", 
		"RecContentType=text/html", 
		"Referer={Host}/quiz", 
		"Snapshot=t87.inf", 
		"Mode=HTML", 
		LAST);
	lr_end_transaction("Question", LR_AUTO);
	
	// clear body request
	lr_save_string("","quizBody");
	
	// if question contains textarea, add to body request 2xuzufv1q5g68yi=large+text
	for(i = 1;  i <= atoi(lr_eval_string("{textarea_count}")); i++){
		lr_param_sprintf("quizBody", "%s%s=large+text&", lr_eval_string("{quizBody}"), lr_paramarr_idx("textarea",i));
	}
	
	// if question contains inputText, add to body request sx59wtrn75s48qigwqt=short+text
	for(i = 1;  i <= atoi(lr_eval_string("{inputText_count}")); i++){
		lr_param_sprintf("quizBody", "%s%s=short+text&", lr_eval_string("{quizBody}"), lr_paramarr_idx("inputText",i));
	}
	
	// if question contains radio, find the element with longest name in radio elements and add to body request pw1odkwx1sy33=2 
	valueCount = atoi(lr_eval_string("{ValueCount}"));
	for(i = 1; i <= atoi(lr_eval_string("{radioName_count}")); i++){
		for(j = (i - 1) * valueCount + 1; j <= valueCount * i; j++){
			if (strlen(lr_paramarr_idx("radioValue", j)) == 1) {
				lr_param_sprintf("quizBody", "%s%s=%s&", lr_eval_string("{quizBody}"), lr_paramarr_idx("radioName", i), lr_paramarr_idx("radioValue", j));
			}
		}
	}

	// if question contains select, find the element with longest name in select elements and add to body request an2mgdqu828kwpej=2
	for(i = 1; i <= atoi(lr_eval_string("{selectName_count}")); i++){
		for(j = (i - 1) * valueCount + 1; j <= valueCount * i; j++){
			if (strlen(lr_paramarr_idx("selectValue", j)) == 1) {
				lr_param_sprintf("quizBody", "%s%s=%s&", lr_eval_string("{quizBody}"), lr_paramarr_idx("selectName", i), lr_paramarr_idx("selectValue", j));
			}
		}
	}
	
	// if question contains checkbox, find the element with longest name in checkbox elements and add to body request 2vqt9tyib7297l=1	
	elemLength = 1;
	for(i = 1; i <= atoi(lr_eval_string("{checkbox_count}")); i++) {
		if(elemLength <= strlen(lr_paramarr_idx("checkbox", i))){
			elemLength = strlen(lr_paramarr_idx("checkbox", i));
			elemIdx = i;
		}
		if (i % valueCount == 0) {
			lr_param_sprintf("quizBody", "%s%s=1&", lr_eval_string("{quizBody}"), lr_paramarr_idx("checkbox", elemIdx));
			elemLength = 1;
		}
	}
	lr_param_sprintf("quizBody", "%s__timestamp=%s&", lr_eval_string("{quizBody}"), lr_eval_string("{timestamp}"));
	lr_param_sprintf("quizBody", "%s__secret=%s", lr_eval_string("{quizBody}"), lr_eval_string("{secret}"));
	
//28 ["redirect","\/quiz\/question\/1",false] 0
	web_reg_save_param(
		"quizNumber", 
		"LB=quiz\\/question\\/", 
		"RB=\"",
		LAST);	

	lr_think_time(10);

	lr_start_transaction("QuizRequest");
	web_custom_request("quiz",
		"URL={Host}/quiz/question/{quizNumber}", 
		"Method=POST", 
		"Resource=0", 
		"RecContentType=application/json", 
		"Referer={Host}/quiz", 
		"Snapshot=t21.inf", 
		"Mode=HTTP", 
		"EncType=application/x-www-form-urlencoded; charset=UTF-8", 
		"Body={quizBody}", 
		LAST);
	lr_end_transaction("QuizRequest", LR_AUTO);
}

	web_reg_find("Text=Congratulations, you've answered all the questions correctly!", 
		LAST);

	lr_think_time(10);

	lr_start_transaction("QuizSubmit");
	web_custom_request("submit", 
		"URL={Host}/quiz/question/{quizNumber}", 
		"Method=GET", 
		"Resource=0", 
		"RecContentType=text/html", 
		"Referer={Host}/quiz/", 
		"Snapshot=t67.inf", 
		"Mode=HTTP", 
		LAST);
	lr_end_transaction("QuizSubmit", LR_AUTO);

	return 0;
}