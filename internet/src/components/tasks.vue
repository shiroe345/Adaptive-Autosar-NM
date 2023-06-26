<template>
    <br/>
    <span test-data = "toggleTaskState">
        <button type = "button" @click = "this.$emit('toggleTaskState',index);"  >Ok</button>
    </span>

        <span v-if="active" test-data="showDone" class = "done">{{messageInComponent}}</span>
        <span v-else test-data="showUndone">{{messageInComponent}}</span>

    <span test-data = "deleteTask">
        <button type="button" @click="this.$emit('deleteCertainTask',time);">delete</button>
    </span>
    <input test-data = "tasksName" v-model="messageNewName">

    <span test-data = "changeTaskName">
        <button type = "button" @click = "fixTaskName();"> fixTaskName </button>
    </span>
     
</template>

<script>

export default{
    name : 'tasksComponents',
    props: ['message', 'active','index','time'],
    data() {
        return {
            Active: this.active,
            messageInComponent: this.message,
            messageNewName : 'none'
        }
    },
    methods: {
        fixTaskName() {
            var updateMessagePack = [];
            updateMessagePack.push({
                "message": this.messageNewName,
                "time": this.time
            });
            this.$forceUpdate();
            this.$emit('fixTaskName', updateMessagePack);
            
        }
    }
}
</script>
<style>
.done {
  color: red;
}
</style>