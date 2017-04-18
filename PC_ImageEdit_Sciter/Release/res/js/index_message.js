/**
 * Created by 11878 on 2017/4/18.
 */

function onResizeFrame(mWidth,mHeight)
{
    gc();
    $(#main_frame).style["width"] = (mWidth-25) + "px";
    gc();
    $(#main_frame).style["height"] = (mHeight-40) + "px";
}
