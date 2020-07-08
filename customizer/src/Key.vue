<template>
    <div class="key" :style="{'--color': bg}">
        <slot></slot>
        <div v-if="!selected" class="edit" @click="start()">&#x1f589;&#xFE0E;</div>
        <div class="popout" v-if="selected">
             <fieldset>
                <legend>passive</legend>
                <select v-model="passiveEffect">
                    <optgroup label="disabled">
                        <option value="PASSIVE_NONE">none</option>
                    </optgroup>
                    <optgroup label="basic">
                        <option value="PASSIVE_SET">set color</option>
                    </optgroup>
                    <optgroup label="effects">
                        <option value="PASSIVE_BLINK">blink</option>
                        <option value="PASSIVE_BREATHE">breathe</option>
                        <option value="PASSIVE_COLOR_CYCLE">color cycle</option>
                    </optgroup>
                </select>
                <input type="color" v-model="passiveColor">
             </fieldset>
            <fieldset>
                <legend>active</legend>
                <select v-model="activeEffect">
                    <optgroup label="disabled">
                        <option value="ACTIVE_NONE">none</option>
                    </optgroup>
                    <optgroup label="basic">
                        <option value="ACTIVE_SET">set color</option>
                    </optgroup>
                    <optgroup label="effects">
                        <option value="ACTIVE_BLINK">blink</option>
                        <option value="ACTIVE_BREATHE">breathe</option>
                        <option value="ACTIVE_COLOR_CYCLE">color cycle</option>
                        <option value="ACTIVE_RIPPLE">ripple</option>
                        <option value="ACTIVE_INV_RIPPLE">inward ripple</option>
                        <option value="ACTIVE_LASER">laser</option>
                    </optgroup>
                </select>
                <input type="color" v-model="activeColor">
             </fieldset>
            <button @click.prevent="finish()">save</button>
        </div>
    </div>
</template>

<script>
    /*
    [n.SET_COLOR=0]="SET_COLOR",
    n[n.BLINK=1]="BLINK",
    n[n.BREATHE=2]="BREATHE",
    n[n.RIPPLE=3]="RIPPLE",
    n[n.LASER=4]="LASER",
    n[n.WAVE=5]="WAVE",
    n[n.COLOR_CYCLE=6]="COLOR_CYCLE",
    n[n.NONE=7]="NONE",
    n[n.MIXED=8]="MIXED",
    n[n.INWARD_RIPPLE=9]="INWARD_RIPPLE",
    n[n.SINUS=10]="SINUS",
    n[n.BOUNCING_LIGHT=11]="BOUNCING_LIGHT"
     */
    import tinycolor from 'tinycolor2'

    export default {
        name: "Key",
        props: {
            code: {
                type: String,
                default: false
            },
            config: {
                type: Object
            }
        },

        data() {
            return {
                selected: false,
                passiveEffect: "NONE",
                passiveColor: "",
                activeEffect: "NONE",
                activeColor: "" ,
            }
        },
        methods: {
            start () {
                this.passiveEffect = this.config.passive.effect
                this.passiveColor = "#" + tinycolor({r: this.config.passive.r, g: this.config.passive.g,b: this.config.passive.b}).toHex()

                this.activeEffect = this.config.active.effect
                this.activeColor = "#" + tinycolor({r: this.config.active.r, g: this.config.active.g,b: this.config.active.b}).toHex()

                this.selected = true;
            },
            finish () {
                let pcolor = tinycolor(this.passiveColor).toRgb()
                let acolor = tinycolor(this.activeColor).toRgb()

                this.$emit("update", {
                    passive: {
                        effect: this.passiveEffect,
                        r: pcolor.r, g:pcolor.g, b:pcolor.b
                    },
                    active: {
                        effect: this.activeEffect,
                        r: acolor.r, g:acolor.g, b:acolor.b
                    }
                })

                this.selected = false;
            }
        },
        computed: {
            bg () {
                return [this.config.passive.r, this.config.passive.g, this.config.passive.b].join(',')
            }
        }
    }
</script>

<style>
    .key {
        position: relative;
    }
    .key .edit {
        cursor: pointer;
        line-height: 10px;
        position: absolute;
        right: 0;
        bottom: 3px;
        display: none;
    }
    .key:hover .edit {
        display: block;
    }

    .popout {
        --offset: 5px;
        background: #fff;
        border-radius: 2px;
        box-shadow: 0 0 2px rgba(0,0,0,.3), 0 4px 8px rgba(0,0,0,.3);
        box-sizing: initial;
        --width: 225px;
        width: var(--width);
        position: absolute;
        left: calc(50% - var(--width) / 2);
        top: calc(100% + var(--offset));
        z-index: 100;
        display: flex;
        color: black;
        padding: 5px;
    }
    .popout:before{
        display: block;
        content: "";
        position: absolute;
        width: 0;
        height: 0;
        border-style: solid;
        border-width: 0 calc(2 * var(--offset)) calc(2 * var(--offset)) calc(2 * var(--offset));
        border-color: transparent transparent #ffffff transparent;
        top: calc(-2 * var(--offset));
        left: calc(50% - 2 * var(--offset));
    }
</style>