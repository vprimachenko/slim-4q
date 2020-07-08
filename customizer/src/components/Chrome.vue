<template>
    <div aria-label="color picker" class="vc-chrome">
        <div class="vc-chrome-saturation-wrap">
            <saturation v-model="colors" @change="childChange"></saturation>
        </div>
        <div class="vc-chrome-body">
            <div class="vc-chrome-controls">
                <div class="vc-chrome-sliders">
                    <div class="vc-chrome-hue-wrap">
                        <hue v-model="colors" @change="childChange"></hue>
                    </div>
                </div>
            </div>

            <div class="vc-chrome-fields-wrap" v-if="!disableFields">
                <div class="vc-chrome-fields" v-show="fieldsIndex === 0">
                    <!-- hex -->
                    <div class="vc-chrome-field">
                        <ed-in label="hex" :value="colors.hex" @change="inputChange"></ed-in>
                    </div>
                </div>
                <div class="vc-chrome-fields" v-show="fieldsIndex === 1">
                    <!-- rgba -->
                    <div class="vc-chrome-field">
                        <ed-in label="r" :value="colors.rgba.r" @change="inputChange"></ed-in>
                    </div>
                    <div class="vc-chrome-field">
                        <ed-in label="g" :value="colors.rgba.g" @change="inputChange"></ed-in>
                    </div>
                    <div class="vc-chrome-field">
                        <ed-in label="b" :value="colors.rgba.b" @change="inputChange"></ed-in>
                    </div>
                </div>
                <div class="vc-chrome-fields" v-show="fieldsIndex === 2">
                    <!-- hsla -->
                    <div class="vc-chrome-field">
                        <ed-in label="h" :value="hsl.h" @change="inputChange"></ed-in>
                    </div>
                    <div class="vc-chrome-field">
                        <ed-in label="s" :value="hsl.s" @change="inputChange"></ed-in>
                    </div>
                    <div class="vc-chrome-field">
                        <ed-in label="l" :value="hsl.l" @change="inputChange"></ed-in>
                    </div>
                </div>
                <!-- btn -->
                <div class="vc-chrome-toggle-btn" role="button" aria-label="Change another color definition" @click="toggleViews">
                    <div class="vc-chrome-toggle-icon">
                        <svg style="width:24px; height:24px" viewBox="0 0 24 24"
                             @mouseover="showHighlight"
                             @mouseenter="showHighlight"
                             @mouseout="hideHighlight">
                            <path fill="#333" d="M12,18.17L8.83,15L7.42,16.41L12,21L16.59,16.41L15.17,15M12,5.83L15.17,9L16.58,7.59L12,3L7.41,7.59L8.83,9L12,5.83Z" />
                        </svg>
                    </div>
                    <div class="vc-chrome-toggle-icon-highlight" v-show="highlight"></div>
                </div>
                <!-- btn -->
            </div>
        </div>

        <ul class="vc-compact-colors" role="listbox">
            <li
                    v-for="c in paletteUpperCase(palette)"
                    role="option"
                    :aria-label="'color:' + c"
                    :aria-selected="c === pick"
                    class="vc-compact-color-item"
                    :key="c"
                    :class="{'vc-compact-color-item--white': c === '#FFFFFF' }"
                    :style="{background: c}"
                    @click="handlerClick(c)"
            >
                <div class="vc-compact-dot" v-show="c === pick"></div>
            </li>
        </ul>
    </div>
</template>

<script>
    import colorMixin from '../mixin/color'
    import editableInput from './common/EditableInput.vue'
    import saturation from './common/Saturation.vue'
    import hue from './common/Hue.vue'

    const defaultColors = [
        '#FFFFFF', '#F44E3B', '#FE9200', '#FCDC00', '#DBDF00', '#A4DD00', '#68CCCA', '#73D8FF', '#AEA1FF', '#FDA1FF',
        '#808080', '#D33115', '#E27300', '#FCC400', '#B0BC00', '#68BC00', '#16A5A5', '#009CE0', '#7B64FF', '#FA28FF',
        '#000000', '#9F0500', '#C45100', '#FB9E00', '#808900', '#194D33', '#0C797D', '#0062B1', '#653294', '#AB149E'
    ]

    export default {
        name: 'Chrome',
        mixins: [colorMixin],
        props: {
            disableFields: {
                type: Boolean,
                default: false
            },
            palette: {
                type: Array,
                default () {
                    return defaultColors
                }
            }
        },
        components: {
            saturation,
            hue,
            'ed-in': editableInput
        },
        data () {
            return {
                fieldsIndex: 0,
                highlight: false
            }
        },
        computed: {
            hsl () {
                const { h, s, l } = this.colors.hsl
                return {
                    h: h.toFixed(),
                    s: `${(s * 100).toFixed()}%`,
                    l: `${(l * 100).toFixed()}%`
                }
            },
            activeColor () {
                const rgba = this.colors.rgba
                return 'rgba(' + [rgba.r, rgba.g, rgba.b, rgba.a].join(',') + ')'
            },
            hasAlpha () {
                return this.colors.a < 1
            },
            pick () {
                return this.colors.hex.toUpperCase()
            }
        },
        methods: {
            childChange (data) {
                this.colorChange(data)
            },
            inputChange (data) {
                if (!data) {
                    return
                }
                if (data.hex) {
                    this.isValidHex(data.hex) && this.colorChange({
                        hex: data.hex,
                        source: 'hex'
                    })
                } else if (data.r || data.g || data.b || data.a) {
                    this.colorChange({
                        r: data.r || this.colors.rgba.r,
                        g: data.g || this.colors.rgba.g,
                        b: data.b || this.colors.rgba.b,
                        a: data.a || this.colors.rgba.a,
                        source: 'rgba'
                    })
                } else if (data.h || data.s || data.l) {
                    const s = data.s ? (data.s.replace('%', '') / 100) : this.colors.hsl.s
                    const l = data.l ? (data.l.replace('%', '') / 100) : this.colors.hsl.l

                    this.colorChange({
                        h: data.h || this.colors.hsl.h,
                        s,
                        l,
                        source: 'hsl'
                    })
                }
            },
            toggleViews () {
                if (this.fieldsIndex >= 2) {
                    this.fieldsIndex = 0
                    return
                }
                this.fieldsIndex++
            },
            showHighlight () {
                this.highlight = true
            },
            hideHighlight () {
                this.highlight = false
            },
            handlerClick (c) {
                this.colorChange({
                    hex: c,
                    source: 'hex'
                })
            }
        }
    }
</script>

<style>
    .vc-compact {
        padding-top: 5px;
        padding-left: 5px;
        width: 245px;
        border-radius: 2px;
        box-sizing: border-box;
        box-shadow: 0 2px 10px rgba(0,0,0,.12), 0 2px 5px rgba(0,0,0,.16);
        background-color: #fff;
    }
    .vc-compact-colors {
        overflow: hidden;
        padding: 0 10px 10px 15px;
        margin: 0;
    }
    .vc-compact-color-item {
        list-style: none;
        width: 15px;
        height: 15px;
        float: left;
        margin-right: 5px;
        margin-bottom: 5px;
        position: relative;
        cursor: pointer;
    }
    .vc-compact-color-item--white {
        box-shadow: inset 0 0 0 1px #ddd;
    }
    .vc-compact-color-item--white .vc-compact-dot {
        background: #000;
    }

    .vc-compact-dot {
        position: absolute;
        top: 5px;
        right: 5px;
        bottom: 5px;
        left: 5px;
        border-radius: 50%;
        opacity: 1;
        background: #fff;
    }
    .vc-chrome {
        background: #fff;
        border-radius: 2px;
        box-shadow: 0 0 2px rgba(0,0,0,.3), 0 4px 8px rgba(0,0,0,.3);
        box-sizing: initial;
        --width: 225px;
        width: var(--width);
        position: absolute;
        left: calc(50% - var(--width) / 2);
        bottom: -305px;
        z-index: 100;
    }
    .vc-chrome:before{
        display: block;
        content: "";
        position: absolute;
        width: 0;
        height: 0;
        border-style: solid;
        border-width: 0 10px 10px 10px;
        border-color: transparent transparent #ffffff transparent;
        top: -10px;
        left: calc(50% - 10px);
    }

    .vc-chrome-controls {
        display: flex;
    }
    .vc-chrome-color-wrap {
        position: relative;
        width: 36px;
    }
    .vc-chrome-active-color {
        position: relative;
        width: 30px;
        height: 30px;
        border-radius: 15px;
        overflow: hidden;
        z-index: 1;
    }
    .vc-chrome-color-wrap .vc-checkerboard {
        width: 30px;
        height: 30px;
        border-radius: 15px;
        background-size: auto;
    }
    .vc-chrome-sliders {
        flex: 1;
    }
    .vc-chrome-fields-wrap {
        display: flex;
        padding-top: 16px;
    }
    .vc-chrome-fields {
        display: flex;
        margin-left: -6px;
        flex: 1;
    }
    .vc-chrome-field {
        padding-left: 6px;
        width: 100%;
    }
    .vc-chrome-toggle-btn {
        width: 32px;
        text-align: right;
        position: relative;
    }
    .vc-chrome-toggle-icon {
        margin-right: -4px;
        margin-top: 12px;
        cursor: pointer;
        position: relative;
        z-index: 2;
    }
    .vc-chrome-toggle-icon-highlight {
        position: absolute;
        width: 24px;
        height: 28px;
        background: #eee;
        border-radius: 4px;
        top: 10px;
        left: 12px;
    }
    .vc-chrome-hue-wrap {
        position: relative;
        height: 10px;
        margin-bottom: 8px;
    }
    .vc-chrome-alpha-wrap {
        position: relative;
        height: 10px;
    }
    .vc-chrome-hue-wrap .vc-hue {
        border-radius: 2px;
    }
    .vc-chrome-alpha-wrap .vc-alpha-gradient {
        border-radius: 2px;
    }
    .vc-chrome-hue-wrap .vc-hue-picker, .vc-chrome-alpha-wrap .vc-alpha-picker {
        width: 12px;
        height: 12px;
        border-radius: 6px;
        transform: translate(-6px, -2px);
        background-color: rgb(248, 248, 248);
        box-shadow: 0 1px 4px 0 rgba(0, 0, 0, 0.37);
    }
    .vc-chrome-body {
        padding: 16px 16px 12px;
        background-color: #fff;
    }
    .vc-chrome-saturation-wrap {
        width: 100%;
        padding-bottom: 55%;
        position: relative;
        border-radius: 2px 2px 0 0;
        /*overflow: hidden;*/
    }


    .vc-chrome-fields .vc-input__input {
        font-size: 11px;
        color: #333;
        width: 100%;
        border-radius: 2px;
        border: none;
        box-shadow: inset 0 0 0 1px #dadada;
        height: 21px;
        text-align: center;
    }
    .vc-chrome-fields .vc-input__label {
        text-transform: uppercase;
        font-size: 11px;
        line-height: 11px;
        color: #969696;
        text-align: center;
        display: block;
        margin-top: 12px;
    }

    .vc-chrome__disable-alpha .vc-chrome-active-color {
        width: 18px;
        height: 18px;
    }
    .vc-chrome__disable-alpha .vc-chrome-color-wrap {
        width: 30px;
    }
    .vc-chrome__disable-alpha .vc-chrome-hue-wrap {
        margin-top: 4px;
        margin-bottom: 4px;
    }
</style>
