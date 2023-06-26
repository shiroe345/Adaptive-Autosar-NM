<template>
    <h3>tasks:{{countUndoneTasks}} tasks Done:{{countDoneTasks}} </h3>
    <br/>
    <span test-data = "allTask">
        <button type="button" @click="deleteAllTasks" > delete all tasks  </button>
    </span>
    <span test-data = "doneTask">
        <button type="button" @click="deleteDoneTask"> delete done tasks </button>
    </span>
    <br/>
    <buttonDecideElementToShow :showValue="this.showValue" @setShowValue = "setShowValue"></buttonDecideElementToShow>
    <span test-data = "create">
        <button type="button" @click="create">add</button>
    </span>
    <input test-data = "taskName" v-model="message"> {{message}}
    <tasksComponents test-data = "tasks" v-for="element,index in decideWhatToShow"
        :key = "element.time"
        :index = "index"
        :time = "element.time"
        :message = "element.message"
        :active = "element.isActive"
        @deleteCertainTask = "deleteCertainTask"
        @toggleTaskState = "toggleTaskState"
        @fixTaskName = "fixTaskName"
        >
        
    </tasksComponents>

</template>


<script>
    import buttonDecideElementToShow from './components/decideElementToShow.vue'
    import tasksComponents from './components/tasks.vue'

    export default{
        name:'listToDoList',
        components: {
            buttonDecideElementToShow,
            tasksComponents
        },
    data() {
        return{
            message : "temp",
            show: "all",
            undoneTasks: 0,
            doneTasks: 0, 
            list: []
        }
    },
    methods: {
        forceToUpdate(){
            for (var y = 0; y < this.list.length; y++) {
                this.list[y].time += 1;
            }
            
        },
        setShowValue(valueShow){
            this.show = valueShow;
        },
        toggleTaskState(index) {
           // console.log("there" + index); 
            this.list[index].isActive = !this.list[index].isActive;
            this.$forceUpdate();
        },
        deleteAllTasks() {
            this.list = [];
        },
        deleteCertainTask(timeValue) {
            var i = 0;
            for (; i < this.list.length; i++) {
                if (this.list[i].time == timeValue) break;
            }
            this.list.splice(i,1);        
            this.forceToUpdate();
        },
        deleteDoneTask() {
            if (this.list.length == 0); 
            else {
                this.list = this.list.filter(i => i.isActive === false);
            }
        },
        create() {
            var time = Date.now();
            this.list.push({
                "message": this.message,
                "time": time,
                "isActive": false //預設是false代表還沒完成
            })
        },
        fixTaskName(updateMessagePack) { // updateMessagePack: [0].time / [0].message
            var i = 0;
            for (; i < this.list.length; i++) {
                if (this.list[i].time == updateMessagePack[0].time) break;
            }
            this.list[i].message = updateMessagePack[0].message;          
            this.forceToUpdate();
        }
    },
    computed: {
        decideWhatToShow () {
            switch(this.show){
                case 'all':
                    return this.list;
                case 'done':
                    return this.list.filter(i => i.isActive === true);
                case 'undone':
                    return this.list.filter(i => i.isActive === false);
                default:
                    return this.list;
            }
        },
        countDoneTasks(){
           return this.list.filter(item => item.isActive).length;
        },
        countUndoneTasks(){
            return this.list.filter(item => !item.isActive).length;
        },
    }
        
}


</script>


<style>
</style>