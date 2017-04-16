
var filelist = [];
var cutImageList = [];
var pictureList = [];
var picturePxList=[];

var pictureListSize = 0;
var curFoucsID = 0;
var rectID = 0;
var curWidth = 700;
var maxWidth = 1400;
var minWidth = 500;
var trackerObj = [];
function alert(msg)
{
    view.msgbox(null,msg+"");
}
function getValue(divID,divAttr)
{
    var m_id = divID.style[divAttr].toString();
    var m_no = m_id.substring(0,m_id.length-2).toInteger();
    return m_no;
}
function printf(msg)
{
    $(#console).append(msg+"<br/>");
}
function init()
{
    filelist = [];
    cutImageList = [];
    pictureList = [];
    picturePxList=[];
    trackerObj = [];
    pictureListSize = 0;
    curFoucsID = 0;
    rectID=0;
}
var Obj=null;
function updateNavBox()
{
    picturePxList = view.getPicPxList();
    //获取每张图片的像素width*height
    pictureListSize = filelist.length;
    for(var i=0; i < pictureListSize; i++)
    {
        var item = {
            rectList:[]
        };
        pictureList.push(item);
    }

    for(var i=0; i < pictureListSize; i++)
    {
        var whrate = picturePxList[i][1].toFloat()/picturePxList[i][0].toFloat();

        var mHeight = (700*whrate).toInteger()+10;
        $(#view_box).$append(<div class="pictureCanvas" #pictureCanvas_{i} id="pictureCanvas_{i}"></div>);
        gc();
        $(#pictureCanvas_{i}).style["background-image"] = "url("+filelist[i]+")";
        gc();
        $(#pictureCanvas_{i}).style["background-size"] = "700px";
        gc();
        $(#pictureCanvas_{i}).style["width"] = "700px";
        gc();
        $(#pictureCanvas_{i}).style["height"] = mHeight+"px";
        var obj = RectTracker($(#pictureCanvas_{i}));
        trackerObj.push(obj);
        $(#pictureCanvas_{i}).on("mouseleave",function(){

        })
        $(#pictureCanvas_{i}).on("mouseenter",function(){
            var m_id = this.attributes["id"].toString();
            var m_no = m_id.substring(14,m_id.length).toInteger();
            curFoucsID = m_no;

        })
    }
}
function QueryDialog()
{
    if(pictureListSize>0)
    {
        var ret = view.msgbox(null, "请先关闭打开的文档",
            "提示",[ {id:#yes, text:"确定"}, {id:#no, text:"取消"} ] );
        if( ret == #yes )
        {
            //清理内存
            for(var i=0;i<pictureListSize;i++)
            {
                $(#pictureCanvas_{i}).remove();
            }
            for(var i=0;i<pictureListSize;i++)
            {
                $(#list_{i}).remove();
            }
            for(var i=0;i<pictureListSize;i++)
            {
                $(#title_page_{i}).remove();
            }

            for(var i=0;i<rectID;i++)
            {
                var obj = $(#rect_{i});
                if(obj)
                    obj.remove();
                obj = null;
            }
            init();
        }
    }
}

function loadProgresInit()
{
    $(#main_frame).$append(<div class="load_progress_box" #load_progress_box> <div class="progress_bar" #progress_bar> <div class="cur_value_bar" #cur_value_bar></div> </div> </div>);
}
function loadProgressCb(m_id,total,pngPath)
{
    var curV = (m_id*1.0/total)*100;
    gc();
    $(#cur_value_bar).style["width"] = curV+"%";
    filelist.push(pngPath);
}
function loadProgressDone(ocr_pageList)
{

    updateNavBox();
    $(#cur_value_bar).remove();
    $(#progress_bar).remove();
    $(#load_progress_box).remove();
    for(var i=0;i<ocr_pageList.length;i++)
    {
        var ocr_rectlist = ocr_pageList[i];
        for(var j=0;j<ocr_rectlist.length;j++)
        {
            $(#pictureCanvas_{i}).$append(<div class="rect_box" #rect_{rectID}  id="rect_{rectID}"></div>);
            var mLeft = ocr_rectlist[j][0];
            var mTop = ocr_rectlist[j][1];
            var mWidth = ocr_rectlist[j][2];
            var mHeight = ocr_rectlist[j][3];

            var g_rate = 700.0/curWidth;
            var orgLeft = (mLeft*g_rate).toInteger();
            var orgTop = (mTop*g_rate).toInteger();
            var orgWidth = (mWidth*g_rate).toInteger();
            var orgHeight = (mHeight*g_rate).toInteger();
            var item = {
                id:rectID,
                left:orgLeft,
                top:orgTop,
                width:orgWidth,
                height:orgHeight
            };
            pictureList[i].rectList.push(item);

            gc();
            $(#rect_{rectID}).style["left"] = mLeft+"px";
            gc();
            $(#rect_{rectID}).style["top"] = mTop +"px";
            gc();
            $(#rect_{rectID}).style["width"] = mWidth+"px";
            gc();
            $(#rect_{rectID}).style["height"] = mHeight +"px";
            rectID++;
        }
    }

    for(var pid=0;pid<rectID;pid++)
    {
        $(#rect_{pid}).on("mousedown",function()
        {

            var tracker = trackerObj[curFoucsID];

            var m_id = this.attributes["id"].toString();
            var m_no = m_id.substring(5,m_id.length).toInteger();
            for(var i=0;i<pictureList[curFoucsID].rectList.length;i++)
            {
                var m_id = pictureList[curFoucsID].rectList[i].id;
                if(m_no == m_id)
                {
                    var g_rate = curWidth/700.0;
                    var orgLeft = (pictureList[curFoucsID].rectList[i].left*g_rate).toInteger();
                    var orgTop = (pictureList[curFoucsID].rectList[i].top*g_rate).toInteger();
                    var orgWidth = (pictureList[curFoucsID].rectList[i].width*g_rate).toInteger();
                    var orgHeight = (pictureList[curFoucsID].rectList[i].height*g_rate).toInteger();
                    tracker.showTracker(orgLeft,orgTop,orgLeft+orgWidth,orgTop+orgHeight);
                    this.remove();
                    break;
                }
            }
        })

    }


}
function get_OCR_init()
{
    $(#edit_box).$append(<div class="ocr_box" #ocr_box></div>)
}
function get_OCR_Pb(percent,questionText)
{
    $(#ocr_box).content(percent+"");
    $(#edit_box).$append(<textarea class="question_text" #cut_{percent} id="cut_{percent}"></textarea>);
    $(#cut_{percent}).appendText(questionText);
}
function get_OCR_Done()
{
    $(#ocr_box).remove();
}
$(#flush_button).on("click",function(){


    var templist=[];
    for(var i=0;i<pictureListSize;i++)
    {
        var rectList=[];
        for(var j=0;j<pictureList[i].rectList.length;j++)
        {
            var m_id = pictureList[i].rectList[j].id.toInteger();
            var obj = $(#rect_{m_id});
            if(obj)
            {
                var item = [];
                item.push(pictureList[i].rectList[j].left);
                item.push(pictureList[i].rectList[j].top);
                item.push(pictureList[i].rectList[j].width);
                item.push(pictureList[i].rectList[j].height);
                rectList.push(item);
            }
        }
        templist.push(rectList);
    }

    view.getImageCutList(templist);
    //alert(cutImageList.length);
    /*
    $(#edit_box).clear();
    for(var i=0;i<cutImageList.length;i++)
    {
        $(#edit_box).$append(<img #cut_{i} id="cut_{i}"/>);
        gc();
        $(#cut_{i}).attributes["src"] = cutImageList[i];
        $(#cut_{i}).on("click",function(){
            //view.dialog();
        });
    }
    */
    /*
    $(#edit_box).clear();

    for(var i=0;i<cutImageList.length;i++)
    {

    }
    */
})
function updateRectListWH(sub)
{
    var mRate = curWidth/700.0;
    for(var i=0;i<pictureListSize;i++)
    {
        for(var j=0;j<pictureList[i].rectList.length;j++)
        {
            var m_id = pictureList[i].rectList[j].id.toInteger();
            var obj = $(#rect_{m_id});
            if(obj)
            {

                var mLeft = pictureList[i].rectList[j].left*mRate;
                var mTop = pictureList[i].rectList[j].top*mRate;
                var mWidth = pictureList[i].rectList[j].width*mRate;
                var mHeight = pictureList[i].rectList[j].height*mRate;
                gc();
                obj.style["left"] = mLeft+"px";
                gc();
                obj.style["top"] = mTop+"px";
                gc();
                obj.style["width"] = mWidth+"px";
                gc();
                obj.style["height"] = mHeight+"px";
            }
        }
    }
}
//Key事件
var is_ctrl = false;
var is_c = false;
function windowKeyHandler() // install movable window handler
{
    function onKeyDown(event) {
        //把新的橡皮筋box加入队列 enter Key
        if (event.keyCode == Event.VK_ENTER) {
            var tracker = trackerObj[curFoucsID];

            var mLeft = tracker.getRectLeft();
            var mTop = tracker.getRectTop();
            var mWidth = tracker.getRectRight() - mLeft;
            var mHeight = tracker.getRectBottom()-mTop;

            if(mWidth > 5 && mHeight > 5)
            {
                gc();
                $(#pictureCanvas_{curFoucsID}).$append(<div class="rect_box" #rect_{rectID}></div>);
                gc();
                $(#rect_{rectID}).style["left"] = mLeft+"px";
                gc();
                $(#rect_{rectID}).style["top"] = mTop +"px";
                gc();
                $(#rect_{rectID}).style["width"] = mWidth+"px";
                gc();
                $(#rect_{rectID}).style["height"] = mHeight +"px";
                gc();

                $(#rect_{rectID}).on("mousedown",function()
                {

                    for(var i=0;i<pictureList[curFoucsID].rectList.length;i++)
                    {
                        var m_id = pictureList[curFoucsID].rectList[i].id;
                        if(this == $(#rect_{m_id}))
                        {
                            var g_rate = curWidth/700.0;
                            var orgLeft = (pictureList[curFoucsID].rectList[i].left*g_rate).toInteger();
                            var orgTop = (pictureList[curFoucsID].rectList[i].top*g_rate).toInteger();
                            var orgWidth = (pictureList[curFoucsID].rectList[i].width*g_rate).toInteger();
                            var orgHeight = (pictureList[curFoucsID].rectList[i].height*g_rate).toInteger();
                            tracker.showTracker(orgLeft,orgTop,orgLeft+orgWidth,orgTop+orgHeight);
                            this.remove();
                            break;
                        }
                    }
                })

                var g_rate = 700.0/curWidth;
                var orgLeft = (mLeft*g_rate).toInteger();
                var orgTop = (mTop*g_rate).toInteger();
                var orgWidth = (mWidth*g_rate).toInteger();
                var orgHeight = (mHeight*g_rate).toInteger();
                var item = {
                    id:rectID,
                    left:orgLeft,
                    top:orgTop,
                    width:orgWidth,
                    height:orgHeight
                };
                pictureList[curFoucsID].rectList.push(item);
                rectID++;
                tracker.hiddenTracker();
            }

        }
        else if(event.keyCode == 192)// ~ key
        {
            //updateRectListWH();
        }
        else if(event.keyCode == 219) //"[" 缩小
        {
            if(curWidth-10>=minWidth)
            {
                curWidth -= 10;

                for(var i=0; i < pictureListSize; i++)
                {

                    var mWidth = curWidth;
                    var whrate = picturePxList[i][1].toFloat()/picturePxList[i][0].toFloat();
                    var mHeight = (mWidth*whrate).toInteger()+10;
                    gc();
                    $(#pictureCanvas_{i}).style["background-size"] = mWidth+"px";
                    gc();
                    $(#pictureCanvas_{i}).style["width"] = mWidth+"px";
                    gc();
                    $(#pictureCanvas_{i}).style["height"] = mHeight+"px";
                }
                updateRectListWH(-5);
            }

        }
        else if(event.keyCode == 221) //"]" 放大
        {
            if(curWidth+10<=maxWidth)
            {

                curWidth += 10;
                for(var i=0; i < pictureListSize; i++)
                {

                    var mWidth = curWidth;
                    var whrate = picturePxList[i][1].toFloat()/picturePxList[i][0].toFloat();
                    var mHeight = (mWidth*whrate).toInteger()+10;
                    gc();
                    $(#pictureCanvas_{i}).style["background-size"] = mWidth+"px";
                    gc();
                    $(#pictureCanvas_{i}).style["width"] = mWidth+"px";
                    gc();
                    $(#pictureCanvas_{i}).style["height"] = mHeight+"px";
                }
                updateRectListWH(5);
            }
        }

        else if(event.keyCode == 46) //"del"
        {

            var tracker = trackerObj[curFoucsID];
            tracker.hiddenTracker();
        }
    }
    function onKeyUp(event)
    {
        if(event.keyCode == 17)
        {
            is_ctrl = true;
        }
        else if(event.keyCode == 67)
        {
            is_c = true;
        }
        if(is_ctrl && is_c)
        {
            is_c = false;
            is_ctrl = false;
            {
                if(pictureListSize==0)
                    return;
                var g_rate = 700.0 / curWidth;

                var rect = [];
                var tracker = trackerObj[curFoucsID];
                var mLeft = (tracker.getRectLeft()*g_rate).toInteger();
                var mTop = (tracker.getRectTop()*g_rate).toInteger();
                var mWidth = (tracker.getRectRight()*g_rate).toInteger() - mLeft;
                var mHeight = (tracker.getRectBottom()*g_rate).toInteger() - mTop;
                if(tracker.isVisible == false)
                {
                    return;
                }
                rect.push(mLeft);
                rect.push(mTop);
                rect.push(mWidth);
                rect.push(mHeight);
                function copyedInit(ti)
                {
                    gc();
                    $(#main_frame).$append(<div class="copyed" #copyed>复制完成</div>);
                    gc();
                    $(#copyed).style["background-color"] = "#555555ff";
                }
                function copyedDone(ti)
                {
                    if(ti==0)
                    {
                        gc();
                        $(#copyed).style["background-color"] = "#55555500";
                    }
                    else if(ti==1)
                    $(#copyed).remove();
                }
                view.copyBmpToClipboard(curFoucsID,rect,copyedInit,copyedDone);

            }
        }
    }
    view.root.subscribe(onKeyDown, Event.KEY, Event.KEY_DOWN );
    view.root.subscribe(onKeyUp, Event.KEY, Event.KEY_UP );
}

windowKeyHandler();
