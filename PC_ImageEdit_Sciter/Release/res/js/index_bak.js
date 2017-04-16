
var filelist = null;
var pictureList = [];
var pictureListID = 0;
var rectList = [];
var rectID = 0;
var sbRate = 1.0;
function alert(msg)
{
    view.msgbox(null,msg);
}
function updateNavBox()
{
    var mLen = filelist.length;
    for(var i=0; i < mLen; i++){
        $(#nav_box).$append(<img id=list_{i} />);
    }
    for(var i=0; i < mLen; i++)
    {
        $(#list_{i}).attributes["src"] = filelist[i];
        $(#list_{i}).attributes["width"] = "80px";
        var m_rectList = [];
        var pictureItem = {
            m_rectList:m_rectList;
        };
        pictureList.push(pictureItem);
    }
    for(var i=0; i < mLen; i++)
    {
        alert(0);
        $(#view_box).$append(<div class="pictureCanvas" #pictureCanvas_{i}><div class="track_edit" #track_edit></div></div>);
        $(#pictureCanvas_{i})..style["background-image"] = "url(file://"+filelist[i]+")";
        $(#pictureCanvas_{i})..style["background-size"] = "700px";
        $(#pictureCanvas_{i})..style["width"] = "700px";
    }
}
self.on("click", "menu > li ", function() {
    var menu_value = this.value;
    if(menu_value=="打开PDF")
    {
        filelist = view.onOpenPdf();
        updateNavBox();
    }
    else if(menu_value == "打开图片")
    {
        filelist = view.onOpenImage();
        updateNavBox();
    }
    else if(menu_value=="关闭"){
        var ret = view.msgbox(null, "确定关闭文档?",
            "提示",[ {id:#yes, text:"确定"}, {id:#no, text:"取消"} ] );
        if( ret == #yes )
        {
            $(#nav_box).clear();
            $(#pictureCanvas).style["background-image"] = "none";
        }
    }
});

$(#nav_box).on("click","img",function(){
    var m_id = this.attributes["id"].toString();
    var m_no = m_id.substring(5,m_id.length).toInteger();

    if(filelist)
    {
        for(var i=0;i<pictureList[pictureListID].m_rectList.length;i++)
        {
            $(#rect_{i}).style["margin-left"] = mLeft+"px";
            $(#rect_{i}).style["margin-top"] = mTop+"px";
            $(#rect_{i}).style["width"] = mWidth+"px";
            $(#rect_{i}).style["height"] = mHeight+"px";
        }
        pictureListID = m_no;
        $(#pictureCanvas).style["background-image"] = "url(file://"+filelist[m_no]+")";
        $(#pictureCanvas).style["background-size"] = "700px";
        $(#pictureCanvas).style["width"] = "700px";
    }
});

self.ready = function() {
    var track_start_x;
    var track_start_y;
    var track_end_x;
    var track_end_y;
    var track_is_drawing = false;

    var track_drag_start_x;
    var track_drag_start_y;
    var track_drag_end_x;
    var track_drag_end_y;
    var track_drag_is_move = false;


    var alt_start_x;
    var alt_start_y;
    var alt_end_x;
    var alt_end_y;
    var alt_isDraw = false;
    var is_alt = false;

    var is_enter = false;
    var is_ctrl = false;
    var is_wheel = 0;
    var view_top = 0;
    var view_left = 0;
    var view_mouse_x;
    var view_mouse_y;
    var cur_rate = 700;
    var max_rate = 1200;
    var min_rate = 500;
    //橡皮筋功能
    $(#track_edit).on("mousedown",function(event){
        track_drag_start_x = event.x;
        track_drag_start_y = event.y;
        track_drag_is_move = true;
        return true;
    })
    $(#track_edit).on("mousemove",function(event){
        if(track_drag_is_move){
            track_drag_end_x = event.x;
            track_drag_end_y = event.y;
            var leftString  = $(#track_edit).style["margin-left"].toString();
            var n_left = leftString.substring(0,leftString.length-2).toInteger();
            var topString  = $(#track_edit).style["margin-top"].toString();
            var n_top = topString.substring(0,topString.length-2).toInteger();
            $(#track_edit).style["margin-left"] =  (n_left + track_drag_end_x -track_drag_start_x)+"px";
            $(#track_edit).style["margin-top"] = (n_top + track_drag_end_y -track_drag_start_y)+"px";
        }
    })
    $(#track_edit).on("mouseup",function(event){
        track_drag_is_move = false;
    })
    $(#track_edit).on("mouseleave",function(event){
        track_drag_is_move = false;
    })
    $(#pictureCanvas).on("mousedown",function(event) {
        track_start_x = event.x;
        track_start_y = event.y;
        track_end_x = event.x;
        track_end_y = event.y;
        track_is_drawing = true;
        return true;
    })
    $(#pictureCanvas).on("mousemove",function(event){
        if(track_is_drawing)
        {
            track_end_x = event.x;
            track_end_y = event.y;
            $(#track_edit).style["margin-left"] = track_start_x+"px";
            $(#track_edit).style["margin-top"] = track_start_y+"px";
            $(#track_edit).style["width"] = (track_end_x - track_start_x)+"px";
            $(#track_edit).style["height"] = (track_end_y - track_start_y)+"px";
        }
    })
    $(#pictureCanvas).on("mouseup",function(event){
        track_is_drawing = false;
    })

    //Mouse event
    function windowMouseHandler()
    {
        function onMouseUp(event)
        {

        }
        view.root.subscribe(onMouseUp, Event.MOUSE, Event.MOUSE_UP );

    }
    windowMouseHandler();
    function updateRectList()
    {

        for(var i=0;i<pictureList[pictureListID].m_rectList.length;i++)
        {
            var mLeft = pictureList[pictureListID].m_rectList[i].left * sbRate;
            var mTop = pictureList[pictureListID].m_rectList[i].top * sbRate;
            var mWidth = pictureList[pictureListID].m_rectList[i].width * sbRate;
            var mHeight = pictureList[pictureListID].m_rectList[i].height * sbRate;
            $(#edit_box>input).content(sbRate+":"+mWidth+","+mHeight);
            $(#rect_{i}).style["margin-left"] = mLeft+"px";
            $(#rect_{i}).style["margin-top"] = mTop+"px";
            $(#rect_{i}).style["width"] = mWidth+"px";
            $(#rect_{i}).style["height"] = mHeight+"px";

        }
    }
    //Key事件
    function windowKeyHandler() // install movable window handler
    {
        function onKeyDown(evt) {
            //alert(evt.keyCode+"");
            if(evt.keyCode == 221 ) // "[" 放大
            {
                if(cur_rate + 5<=max_rate){
                    cur_rate = cur_rate + 5;
                    sbRate = cur_rate/700.0;
                    $(#pictureCanvas).style["width"] = cur_rate+"px";
                    $(#pictureCanvas).style["background-size"] = cur_rate+"px";

                    updateRectList();
                }
            }
            else if(evt.keyCode == 219 ) //"]" 缩小
            {
                if(cur_rate-5>=min_rate) {
                    cur_rate = cur_rate - 5;
                    sbRate = cur_rate/700.0;
                    $(#pictureCanvas).style["width"] = cur_rate+"px";
                    $(#pictureCanvas).style["background-size"] = cur_rate+"px";

                    updateRectList();
                }
            }
            else if(evt.keyCode == Event.VK_CONTROL )
            {
                is_ctrl = true;
            }
            else if(evt.keyCode == 18) //alt
            {
                is_alt = true;
            }
            else if(evt.keyCode == Event.VK_ENTER)
            {
                var mLeftString = $(#track_edit).style["margin-left"].toString();
                var mTopString = $(#track_edit).style["margin-top"].toString();
                var mWidthString = $(#track_edit).style["width"].toString();
                var mHeightString = $(#track_edit).style["height"].toString();

                var mLeft = mLeftString.substring(0,mLeftString.length-2).toInteger();
                var mTop = mTopString.substring(0,mTopString.length-2).toInteger();
                var mWidth = mWidthString.substring(0,mWidthString.length-2).toInteger();
                var mHeight = mHeightString.substring(0,mHeightString.length-2).toInteger();
                if(mLeft>1 && mTop>1 && mWidth>5 && mHeight>5)
                {
                    var item = {
                        left:mLeft/sbRate,
                        top:mTop/sbRate,
                        width:mWidth/sbRate,
                        height:mHeight/sbRate
                    }
                    //rectList.push(item);
                    pictureList[pictureListID].m_rectList.push(item);


                    $(#pictureCanvas).$append(<div class="rect_box" #rect_{rectID} id=rect_{rectID} ></div>);

                    $(#rect_{rectID}).style["margin-left"] = mLeft+"px";
                    $(#rect_{rectID}).style["margin-top"] = mTop+"px";
                    $(#rect_{rectID}).style["width"] = mWidth+"px";
                    $(#rect_{rectID}).style["height"] = mHeight+"px";

                    rectID++;
                }
                $(#track_edit).style["margin-left"] = "-3px";
                $(#track_edit).style["margin-top"] = "-3px";
                $(#track_edit).style["width"] = "0px";
                $(#track_edit).style["height"] = "0px";

            }

        }
        $(#pictureCanvas).on("click","div",function(){
            var m_id = this.attributes["id"].toString();
            var m_pre = m_id.substring(0,5);
            var m_no = m_id.substring(5,m_id.length).toInteger();
            if(m_pre=="rect_")
            alert(m_no+"");
        });
        function onKeyUp(evt) {
            if(evt.keyCode == Event.VK_CONTROL )
            {
                is_ctrl = false;
            }
            else if(evt.keyCode == 18) //alt
            {
                is_alt = false;
            }

        }

        function onImageWheelPos(event){
            is_wheel = event.wheelDelta;

        }
        view.root.subscribe(onKeyDown, Event.KEY, Event.KEY_DOWN );
        view.root.subscribe(onKeyUp, Event.KEY, Event.KEY_UP );
        //view.root.subscribe(onWheelPos, Event.MOUSE, Event.MOUSE_WHEEL );
        $(#pictureCanvas).subscribe(onImageWheelPos, Event.MOUSE, Event.MOUSE_WHEEL );
        return false;
    }
    windowKeyHandler();


}
