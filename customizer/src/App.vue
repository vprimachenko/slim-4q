<template>
    <div id="app">
        <h1 class="title">Der Configurator</h1>
        <h2 class="subtitle">{{ stateStr }}</h2>
        <svg class="backdrop">
            <rect x="640" width="10" height="105" />
            <path style="fill: #333333" d="M1000,58a 25,25 0 0,1 15,-5
            H1265a 20,20 0 0,1 20,20
            v32
            a 3,3 0 0,1 -4,3
            a 40,40 0 1,0 0,70
            a 3,3 0 0,1 4,3
            v300 a 20,20 0 0,1 -20,20
            H20  a 20,20 0 0,1 -20,-20
            V123  a 20,20 0 0,1 20,-20
            H920 a 25,25 0 0,0 15,-5
            z"></path>
            <circle cx="1261.5" cy="143" r="38.5"/>
        </svg>
        <div class="keyboard">
            <key v-for="key in keys" v-bind:code="key.code" v-bind:config="getConfig(key.code)" :style="{'grid-area': key.code}" @update="setConfig(key.code, $event)">
                <div v-if="key.code == 'KEY_NUMLOCK'" v-html="key.title" class="clickable" @click="flip('NUM_LOCK')"></div>
                <div v-else-if="key.code == 'KEY_CAPS_LOCK'" v-html="key.title" class="clickable" @click="flip('CAPS_LOCK')"></div>
                <div v-else-if="key.code == 'KEY_SHIFT_LEFT'" v-html="key.title" class="clickable" @click="flip('SHIFT')"></div>
                <div v-else-if="key.code == 'KEY_SHIFT_RIGHT'" v-html="key.title" class="clickable" @click="flip('SHIFT')"></div>
                <div v-else-if="key.code == 'KEY_CONTROL_LEFT'" v-html="key.title" class="clickable" @click="flip('CONTROL')"></div>
                <div v-else-if="key.code == 'KEY_CONTROL_RIGHT'" v-html="key.title" class="clickable" @click="flip('CONTROL')"></div>
                <div v-else-if="key.code == 'KEY_ALT_LEFT'" v-html="key.title" class="clickable" @click="flip('MENU')"></div>
                <div v-else-if="key.code == 'KEY_ALT_RIGHT'" v-html="key.title" class="clickable" @click="flip('MENU')"></div>
                <div v-else v-html="key.title"></div>
            </key>
        </div>
        <div class="save" @click="save()">Save config</div>
        <div class="donation">
            <a href="https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=S87VU8TC6KZTG&source=url" target="_blank">i gratefully accept donations to offset licensing costs<br>of software used to create "Der Configurator" <span class="donate">Donate</span></a>
        </div>
<!--        <pre style="overflow-y: scroll;grid-area: 5 / 1 / 6 / 3;height: 300px;text-align: left;text-transform: none;">{{ JSON.stringify(this.config, "\t", 1) }}</pre>-->
    </div>
</template>

<script>
    import Key from './Key.vue'
    import Vue from 'vue';

    function stateToMask(key) {
        let k = 0;
        if (key.includes("NUM_LOCK")) k |= 1 << 0;
        if (key.includes("CAPS_LOCK")) k |= 1 << 1;
        if (key.includes("CONTROL")) k |= 1 << 2;
        if (key.includes("MENU")) k |= 1 << 3;
        if (key.includes("SHIFT")) k |= 1 << 4;
        return k
    }

    function findConfig(_config, _state) {
        let config = Object.entries(_config)
            .map((a) => {
                let [key, value] = a
                return [stateToMask(key), value]
            })
            .sort((a, b) => {
                return b[0] - a[0]
            })

        let state = Object.entries(_state)
            .filter(a => a[1])
            .map(a => stateToMask(a[0]))
            .reduce((a,b) => a+b, 0)

        let c = config.filter((a) => {
            let [key, value] = a
            return (key & ~state) == 0
        }).map(a=>a[1])

        return c[0]
    }

    export default {
        components: {
            Key
        },
        data () {
            return {
                keys : [
                    { code:"KEY_ESCAPE", title: "ESC"},
                    { code:"KEY_F1", title: "F1"},
                    { code:"KEY_F2", title: "F2"},
                    { code:"KEY_F3", title: "F3"},
                    { code:"KEY_F4", title: "F4"},
                    { code:"KEY_F5", title: "F5"},
                    { code:"KEY_F6", title: "F6"},
                    { code:"KEY_F7", title: "F7"},
                    { code:"KEY_F8", title: "F8"},
                    { code:"KEY_F9", title: "F9"},
                    { code:"KEY_F10", title: "F10"},
                    { code:"KEY_F11", title: "F11"},
                    { code:"KEY_F12", title: "F12"},
                    { code:"KEY_BACKTICK", title: "`<br>~"},
                    { code:"KEY_1", title: "1<br>!"},
                    { code:"KEY_2", title: "2<br>@"},
                    { code:"KEY_3", title: "3<br>#"},
                    { code:"KEY_4", title: "4<br>$"},
                    { code:"KEY_5", title: "5<br>%"},
                    { code:"KEY_6", title: "6<br>^"},
                    { code:"KEY_7", title: "7<br>&"},
                    { code:"KEY_8", title: "8<br>*"},
                    { code:"KEY_9", title: "9<br>("},
                    { code:"KEY_0", title: "0<br>)"},
                    { code:"KEY_SUBTRACT", title: "-<br>_"},
                    { code:"KEY_EQUAL", title: "=<br>+"},
                    { code:"KEY_Q", title: "<big>Q</big>"},
                    { code:"KEY_W", title: "<big>W</big>"},
                    { code:"KEY_E", title: "<big>E</big>"},
                    { code:"KEY_R", title: "<big>R</big>"},
                    { code:"KEY_T", title: "<big>T</big>"},
                    { code:"KEY_Y", title: "<big>Y</big>"},
                    { code:"KEY_U", title: "<big>U</big>"},
                    { code:"KEY_I", title: "<big>I</big>"},
                    { code:"KEY_O", title: "<big>O</big>"},
                    { code:"KEY_P", title: "<big>P</big>"},
                    { code:"KEY_OPEN_SQUARE_BRACKET", title: "[<br>{"},
                    { code:"KEY_CLOSE_SQUARE_BRACKET", title: "]<br>}"},
                    { code:"KEY_BACKSLASH", title: "\\<br>|"},
                    { code:"KEY_A", title: "<big>A</big>"},
                    { code:"KEY_S", title: "<big>S</big>"},
                    { code:"KEY_D", title: "<big>D</big>"},
                    { code:"KEY_F", title: "<big>F</big>"},
                    { code:"KEY_G", title: "<big>G</big>"},
                    { code:"KEY_H", title: "<big>H</big>"},
                    { code:"KEY_J", title: "<big>J</big>"},
                    { code:"KEY_K", title: "<big>K</big>"},
                    { code:"KEY_L", title: "<big>L</big>"},
                    { code:"KEY_SEMICOLON", title: ";<br>:"},
                    { code:"KEY_QUOTE", title: "'<br>\""},
                    { code:"KEY_Z", title: "<big>Z</big>"},
                    { code:"KEY_X", title: "<big>X</big>"},
                    { code:"KEY_C", title: "<big>C</big>"},
                    { code:"KEY_V", title: "<big>V</big>"},
                    { code:"KEY_B", title: "<big>B</big>"},
                    { code:"KEY_N", title: "<big>N</big>"},
                    { code:"KEY_M", title: "<big>M</big>"},
                    { code:"KEY_COMMA", title: ",<br><"},
                    { code:"KEY_DOT", title: ".<br>>"},
                    { code:"KEY_SLASH", title: "/<br>?"},
                    { code:"KEY_TAB", title: "|⬌|"},
                    { code:"KEY_CAPS_LOCK", title: "⇪"},
                    { code:"KEY_SHIFT_LEFT", title: "🠭"},
                    { code:"KEY_CONTROL_LEFT", title: "CTRL"},
                    { code:"KEY_META_LEFT", title: "WIN"},
                    { code:"KEY_ALT_LEFT", title: "ALT"},
                    { code:"KEY_SPACE", title: " "},
                    { code:"KEY_ALT_RIGHT", title: "ALT"},
                    { code:"KEY_CONTEXT_MENU", title: "≡"},
                    { code:"KEY_META_RIGHT", title: "WIN"},
                    { code:"KEY_CONTROL_RIGHT", title: "CTRL"},
                    { code:"KEY_SHIFT_RIGHT", title: "🠭"},
                    { code:"KEY_ENTER", title: "↵"},
                    { code:"KEY_BACKSPACE", title: "🠬"},
                    { code:"KEY_PRINT_SCREEN", title: "<small>PRT SC</small>"},
                    { code:"KEY_SCROLL_LOCK", title: "<small>SCR LK</small>"},
                    { code:"KEY_PAUSE_BREAK", title: "<small>PAUSE</small>"},
                    { code:"KEY_INSERT", title: "<small>INS</small>"},
                    { code:"KEY_HOME", title: "<small>HOME</small>"},
                    { code:"KEY_PAGE_UP", title: "<small>PG \u2bc5</small>"},
                    { code:"KEY_DELETE", title: "<small>DEL</small>"},
                    { code:"KEY_END", title: "<small>END</small>"},
                    { code:"KEY_PAGE_DOWN", title: "<small>PG \u2bc6</small>"},
                    { code:"KEY_ARROW_UP", title: "\u2bc5"},
                    { code:"KEY_ARROW_LEFT", title: "\u2bc7"},
                    { code:"KEY_ARROW_DOWN", title: "\u2bc6"},
                    { code:"KEY_ARROW_RIGHT", title: "\u2bc8"},
                    { code:"KEY_NUMLOCK", title: "<small>NUM</small>"},
                    { code:"KEY_NUMPAD_1", title: "1<br><small>END</small>"},
                    { code:"KEY_NUMPAD_2", title: "2<br><small>\u2bc6</small>"},
                    { code:"KEY_NUMPAD_3", title: "3<br><small>PG DN</small>"},
                    { code:"KEY_NUMPAD_4", title: "4<br><small>\u2bc7</small>"},
                    { code:"KEY_NUMPAD_5", title: "5"},
                    { code:"KEY_NUMPAD_6", title: "6<br><small>\u2bc8</small>"},
                    { code:"KEY_NUMPAD_7", title: "7<br><small>HOME</small>"},
                    { code:"KEY_NUMPAD_8", title: "8<br><small>\u2bc5</small>"},
                    { code:"KEY_NUMPAD_9", title: "9<br><small>PG UP</small>"},
                    { code:"KEY_NUMPAD_0", title: "0<br><small>INS</small>"},
                    { code:"KEY_NUMPAD_DECIMAL", title: ".<br>DEL"},
                    { code:"KEY_NUMPAD_ENTER", title: "↵"},
                    { code:"KEY_NUMPAD_ADD", title: "+"},
                    { code:"KEY_NUMPAD_SUBTRACT", title: "-"},
                    { code:"KEY_NUMPAD_MULTIPLY", title: "*"},
                    { code:"KEY_NUMPAD_DIVIDE", title: "/"},
                    { code:"KEY_SLEEP", title: "☾"},
                    { code:"KEY_VOLUME_Q_BUTTON", title: "ϙ"},
                    { code:"KEY_BRIGHTNESS_KEYBOARD", title: "\ue449"},
                    { code:"KEY_PLAY", title: "\u23EF"},
                    { code:"KEY_NEXT", title: "⏭"}
                ],
                state: {"NUM_LOCK": false, "CAPS_LOCK": false, "CONTROL": false, "MENU": false, "SHIFT": false},
                config: {}
            }
        },
        mounted: function() {
            Vue.axios.get("/config.json")
                .then((response) => {
                    this.config = response.data
                });
        },
        methods: {
            flip (key ) {
                this.state[key] = !this.state[key]
            },
            getConfig (key) {
                let cfg = this.config[key]
                let ret = {
                    passive: {
                        effect: "NONE",
                        r: 0, g:0, b:0
                    },
                    active: {
                        effect: "NONE",
                        r: 0, g:0, b:0
                    }
                }

                if (cfg && cfg.passive) {
                    let x = findConfig(cfg.passive, this.state)
                    if (x) {
                        ret.passive = x
                    }
                }
                if (cfg && cfg.active) {
                    let x = findConfig(cfg.active, this.state)
                    if (x) {
                        ret.active = x
                    }
                }

                return ret
            },
            setConfig (code, keyConfig) {
                if (!this.config[code]) {
                    this.$set(this.config, code, {})
                }

                if (keyConfig.passive.effect == "NONE" && this.config[code].passive) {
                    delete this.config[code].passive[this.stateStr]
                } else {
                    if(!this.config[code].passive) {
                        this.$set(this.config[code], "passive", {})
                    }
                    this.$set(this.config[code].passive, this.stateStr, keyConfig.passive)
                }

                if (keyConfig.active.effect == "NONE" && this.config[code].active) {
                    delete this.config[code].active[this.stateStr]
                } else {
                    if(!this.config[code].active) {
                        this.$set(this.config[code], "active", {})
                    }
                    this.$set(this.config[code].active, this.stateStr, keyConfig.active)
                }
            },
            save () {
                Vue.axios.post("/config.json", this.config)
            }
        },
        computed: {
            stateStr() {
                let str = Object.entries(this.state)
                .filter(a => a[1])
                .map(a => a[0])
                .join("+")

                if (str.length) {
                    return str
                } else {
                    return "default"
                }
            }
        }
    }
</script>

<style>
    * {
        margin: 0; padding: 0; box-sizing: border-box;
    }
    @font-face {
      font-family: 'dasfont';
      src: url('assets/fonts/dasfont.woff2') format('woff2');
      font-weight: normal;
      font-style: normal;
      font-display: block;
    }
    body {
        font-family: "dasfont";
        background: #b9c0c7;
    }


    #app {
        -webkit-font-smoothing: antialiased;
        -moz-osx-font-smoothing: grayscale;
        text-align: center;
        color: #6f6f6f;
        width: 1285px;
        position: relative;
        margin: 0 auto;

        display: grid;
        grid-template-columns: 35px 1fr 180px 1fr 35px;
        grid-template-rows: 50px 50px 1fr 10px 43px;
        gap: 1px 1px;
        grid-template-areas: ". title . . ." ". subtitle . . ." ". . . . ."  ". . . . ." ". . save donation .";
    }

    .title { grid-area: title; }
    .subtitle { grid-area: subtitle; }
    .title, .subtitle {
        text-align: left;
        line-height: 50px;
    }

    .backdrop { grid-area: 1 / 1 / 4 / 6; }
    .backdrop {
        width: 1302px;
        height: 500px;
        z-index: -1;
    }

    .keyboard { grid-area: 3 / 2 / 4 / 6; }
    .keyboard {
        display: grid;
        grid-template-columns: 1fr repeat(28, 0.5fr) 0.5fr repeat(3, 1fr) 0.5fr repeat(4, 1fr);
        grid-template-rows: 0.5fr 0.5fr 0.5fr 0.5fr repeat(5, 1fr);
        gap: 5px 5px;
        grid-template-areas:
                "  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .   .  .   .   .   .   .   .   . .  .   .   .  . KEY_SLEEP  . KEY_VOLUME_Q_BUTTON  ."
                "KEY_ESCAPE  .  . KEY_F1 KEY_F1 KEY_F2 KEY_F2 KEY_F3 KEY_F3 KEY_F4 KEY_F4  . KEY_F5 KEY_F5 KEY_F6 KEY_F6 KEY_F7 KEY_F7 KEY_F8 KEY_F8  .  KEY_F9 KEY_F9 KEY_F10 KEY_F10 KEY_F11 KEY_F11 KEY_F12 KEY_F12 . KEY_PRINT_SCREEN  KEY_SCROLL_LOCK  KEY_PAUSE_BREAK  .  .  .  .  ."
                "KEY_ESCAPE  .  . KEY_F1 KEY_F1 KEY_F2 KEY_F2 KEY_F3 KEY_F3 KEY_F4 KEY_F4  . KEY_F5 KEY_F5 KEY_F6 KEY_F6 KEY_F7 KEY_F7 KEY_F8 KEY_F8  .  KEY_F9 KEY_F9 KEY_F10 KEY_F10 KEY_F11 KEY_F11 KEY_F12 KEY_F12 . KEY_PRINT_SCREEN  KEY_SCROLL_LOCK  KEY_PAUSE_BREAK  . KEY_BRIGHTNESS_KEYBOARD KEY_PLAY KEY_NEXT  ."
                ".    .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .   .  .   .   .   .   .   .   . .  .   .   .  .  .  .  .  ."
                "KEY_BACKTICK  KEY_1 KEY_1 KEY_2 KEY_2 KEY_3 KEY_3 KEY_4 KEY_4 KEY_5 KEY_5 KEY_6 KEY_6 KEY_7 KEY_7 KEY_8 KEY_8 KEY_9 KEY_9 KEY_0 KEY_0  KEY_SUBTRACT KEY_SUBTRACT  KEY_EQUAL  KEY_EQUAL  KEY_BACKSPACE  KEY_BACKSPACE  KEY_BACKSPACE  KEY_BACKSPACE . KEY_INSERT KEY_HOME KEY_PAGE_UP . KEY_NUMLOCK KEY_NUMPAD_DIVIDE KEY_NUMPAD_MULTIPLY KEY_NUMPAD_SUBTRACT"
                "KEY_TAB  KEY_TAB KEY_Q KEY_Q KEY_W KEY_W KEY_E KEY_E KEY_R KEY_R KEY_T KEY_T KEY_Y KEY_Y KEY_U KEY_U KEY_I KEY_I KEY_O KEY_O KEY_P  KEY_P KEY_OPEN_SQUARE_BRACKET  KEY_OPEN_SQUARE_BRACKET  KEY_CLOSE_SQUARE_BRACKET  KEY_CLOSE_SQUARE_BRACKET  KEY_BACKSLASH  KEY_BACKSLASH  KEY_BACKSLASH . KEY_DELETE KEY_END KEY_PAGE_DOWN . KEY_NUMPAD_7 KEY_NUMPAD_8 KEY_NUMPAD_9 KEY_NUMPAD_ADD"
                "KEY_CAPS_LOCK  KEY_CAPS_LOCK KEY_CAPS_LOCK KEY_A KEY_A KEY_S KEY_S KEY_D KEY_D KEY_F KEY_F KEY_G KEY_G KEY_H KEY_H KEY_J KEY_J KEY_K KEY_K KEY_L KEY_L  KEY_SEMICOLON KEY_SEMICOLON  KEY_QUOTE  KEY_QUOTE  KEY_ENTER  KEY_ENTER  KEY_ENTER  KEY_ENTER .  .   .   .  . KEY_NUMPAD_4 KEY_NUMPAD_5 KEY_NUMPAD_6 KEY_NUMPAD_ADD"
                "KEY_SHIFT_LEFT  KEY_SHIFT_LEFT KEY_SHIFT_LEFT KEY_SHIFT_LEFT KEY_Z KEY_Z KEY_X KEY_X KEY_C KEY_C KEY_V KEY_V KEY_B KEY_B KEY_N KEY_N KEY_M KEY_M KEY_COMMA KEY_COMMA KEY_DOT  KEY_DOT KEY_SLASH  KEY_SLASH  KEY_SHIFT_RIGHT  KEY_SHIFT_RIGHT  KEY_SHIFT_RIGHT  KEY_SHIFT_RIGHT  KEY_SHIFT_RIGHT .  .  KEY_ARROW_UP   .  . KEY_NUMPAD_1 KEY_NUMPAD_2 KEY_NUMPAD_3 KEY_NUMPAD_ENTER"
                "KEY_CONTROL_LEFT  KEY_CONTROL_LEFT KEY_META_LEFT KEY_META_LEFT KEY_ALT_LEFT KEY_ALT_LEFT KEY_ALT_LEFT KEY_SPACE KEY_SPACE KEY_SPACE KEY_SPACE KEY_SPACE KEY_SPACE KEY_SPACE KEY_SPACE KEY_SPACE KEY_SPACE KEY_SPACE KEY_SPACE KEY_ALT_RIGHT KEY_ALT_RIGHT  KEY_ALT_RIGHT KEY_META_RIGHT  KEY_META_RIGHT  KEY_CONTEXT_MENU  KEY_CONTEXT_MENU  KEY_CONTROL_RIGHT  KEY_CONTROL_RIGHT  KEY_CONTROL_RIGHT . KEY_ARROW_LEFT  KEY_ARROW_DOWN  KEY_ARROW_RIGHT  . KEY_NUMPAD_0 KEY_NUMPAD_0 KEY_NUMPAD_DECIMAL KEY_NUMPAD_ENTER";
        width: 1210px;
        height: 365px;
        user-select: none;
        -moz-user-select: none;
        -webkit-user-select: none;
    }

    .keyboard .key {
        --color: 240, 240, 240;
        background: #101010;
        text-align: center;
        vertical-align: middle;
        color: rgb(var(--color));
        text-shadow: 0 0 4px rgb(var(--color));
        box-shadow: 0 0 4px 1px rgba(var(--color), 0.5);
        border-radius: 3px;
    }
    .keyboard .key {
        font-size: 15px;
        line-height: 24px;
        cursor: default;
    }
    .keyboard .key .clickable {
        cursor: pointer;
    }

    big {
        font-size: 24px;
        line-height: 34px;
    }
    small {
        font-size: 9px;
    }

    .save { grid-area: save; }

    .save {
        cursor: pointer;
        line-height: 43px;
        --color: 240, 240, 240;
        background: #101010;
        text-align: center;
        vertical-align: middle;
        color: rgb(var(--color));
        text-shadow: 0 0 4px rgb(var(--color));
        box-shadow: 0 0 4px 1px rgba(var(--color), 0.5);
        border-radius: 3px;
    }


    .donation { grid-area: donation; }

    .donation {
        display: flex;
        flex-direction: row-reverse;
    }

    .donation a {
        text-decoration: none;
        line-height: 2.5ex;
        margin-right: -10px;
        color: inherit;
    }

    .donate {
        border: 1px solid #6b6b53;
        border-radius: 5px;
        background: linear-gradient(to bottom, #f9ec79 0%,#dc9730 100%);
        color: black;
        padding: 2px 5px;
        margin-right: 10px;
    }
</style>
