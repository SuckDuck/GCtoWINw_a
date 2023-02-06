from flask import Flask, request
import vgamepad as vg
gamepad = vg.VX360Gamepad()

app = Flask(__name__)
def press_buttons(buttons):
    if buttons[0] == "1":
        gamepad.press_button(button=vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_LEFT)
    else: gamepad.release_button(button=vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_LEFT)

    if buttons[1] == "1":
        gamepad.press_button(button=vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_RIGHT)
    else: gamepad.release_button(button=vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_RIGHT)

    if buttons[2] == "1":
        gamepad.press_button(button=vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_DOWN)
    else: gamepad.release_button(button=vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_DOWN)

    if buttons[3] == "1":
        gamepad.press_button(button=vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_UP)
    else: gamepad.release_button(button=vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_UP)

    if buttons[4] == "1":
        gamepad.press_button(button=vg.XUSB_BUTTON.XUSB_GAMEPAD_RIGHT_SHOULDER)
    else: gamepad.release_button(button=vg.XUSB_BUTTON.XUSB_GAMEPAD_RIGHT_SHOULDER)

    if buttons[5] == "1":
        gamepad.right_trigger(value=255)
    else: gamepad.right_trigger(value=0)

    if buttons[6] == "1":
        gamepad.left_trigger(value=255)
    else: gamepad.left_trigger(value=0)

    if buttons[8] == "1":
        gamepad.press_button(button=vg.XUSB_BUTTON.XUSB_GAMEPAD_A)
    else: gamepad.release_button(button=vg.XUSB_BUTTON.XUSB_GAMEPAD_A)

    if buttons[9] == "1":
        gamepad.press_button(button=vg.XUSB_BUTTON.XUSB_GAMEPAD_B)
    else: gamepad.release_button(button=vg.XUSB_BUTTON.XUSB_GAMEPAD_B)

    if buttons[10] == "1":
        gamepad.press_button(button=vg.XUSB_BUTTON.XUSB_GAMEPAD_X)
    else: gamepad.release_button(button=vg.XUSB_BUTTON.XUSB_GAMEPAD_X)

    if buttons[11] == "1":
        gamepad.press_button(button=vg.XUSB_BUTTON.XUSB_GAMEPAD_Y)
    else: gamepad.release_button(button=vg.XUSB_BUTTON.XUSB_GAMEPAD_Y)

    if buttons[12] == "1":
        gamepad.press_button(button=vg.XUSB_BUTTON.XUSB_GAMEPAD_Y)
    else: gamepad.release_button(button=vg.XUSB_BUTTON.XUSB_GAMEPAD_Y)

    gamepad.update()

@app.route("/set_joystick", methods=["GET"])
def get_controller_input():
    if request.args.get("joystick"):
        joystick = request.args.get("joystick")
        #print(joystick[:25])
        press_buttons(joystick[:25])
    return "ok"

if __name__ == "__main__":
    app.run(host="0.0.0.0",debug=True)