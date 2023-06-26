<template>
    <button type="button"  @click = "this.$emit('setShowValue','all');" > show all tasks  </button>
    <button type="button"  @click = "this.$emit('setShowValue','done');" > show done tasks </button>
    <button type="button"  @click = "this.$emit('setShowValue','undone');" > show undone tasks </button>
    <h4></h4>
</template>


<script>

    export default{
        name:'buttonDecideElementToShow',
        components: {
        },
    props: ['showValue'],
    data() {
        return{
            setShowValue : this.showValue
        }
    },
    methods: {

    },
    computed: {
    }
        
}

</script>


<style>
</style>