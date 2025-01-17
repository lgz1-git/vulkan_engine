#include "h_win32_platform.h"
#include "h_inputs.h"
#include "h_global_list.h"

void 
create_window(win32_platform_context* context, win32_config* config)
{
	WNDCLASSEXA wc = {};
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = context->win_instance;
	wc.lpszClassName = config->win_classname;

	assert(RegisterClassExA(&wc) != 0);

	HWND handle = CreateWindowExA(
	        0, 
            config->win_classname, 
            config->win_name,
            WS_SIZEBOX | WS_CAPTION | WS_SYSMENU| WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_VISIBLE,
	        100, 100,
	        config->w, config->h,
	        0, 0, context->win_instance, 0);


	assert(handle != nullptr);
	context->win_handle = handle;

}

static LRESULT
CALLBACK WindowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg) {

    case WM_CLOSE: {
        LTRACE("window close");
        g_running = false;

    }break;
    case WM_DESTROY: {
        LTRACE("window destory");
    }break;
    case WM_SIZE: {
        RECT client_react;
        GetClientRect(wnd, &client_react);
        int32_t width = client_react.right - client_react.left;
        int32_t height = client_react.bottom - client_react.top;
        g_rect_w = width;
        g_rect_h = height;
        LINFO("w: " << g_rect_w);
        LINFO("h: " << g_rect_h);
    }break;
    case WM_SYSKEYUP:
    case WM_SYSKEYDOWN:
    case WM_KEYUP:
    case WM_KEYDOWN: {
            
        bool pressed = (msg == WM_SYSKEYDOWN || msg == WM_KEYDOWN);
        keys key = (keys)wparam;
        input_process_key(key, pressed);
        
    }break;
    case WM_MOUSEMOVE: {
        i32 xPos = GET_X_LPARAM(lparam);
        i32 yPos = GET_Y_LPARAM(lparam);
        input_process_mouse_move(xPos, yPos);
    }break;
    case WM_MOUSEWHEEL: {
        i32 z_delta = GET_WHEEL_DELTA_WPARAM(wparam);
        if (z_delta != 0) {
            z_delta = (z_delta < 0) ? -1 : 1;
        }
        input_process_mouse_wheel(z_delta);
    }break;
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP: {
        bool pressed = msg == WM_LBUTTONDOWN || WM_MBUTTONDOWN || WM_RBUTTONDOWN;
        buttons button = BUTTON_MAX_BUTTON_COUNTS;
        switch (msg)
        {
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP: {
            button = BUTTON_LEFT;
        }break;
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP: {
            button = BUTTON_MIDDLE;
        }break;
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP: {
            button = BUTTON_RIGHT;
        }break;
        }
        if (button != BUTTON_MAX_BUTTON_COUNTS) {
            input_process_button(button, pressed);
        }
    }break;
    case WM_CHAR: {
        //todo::input text

    }
    default:
        return DefWindowProcA(wnd, msg, wparam, lparam);
    }
}







