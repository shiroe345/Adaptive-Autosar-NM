<template>
  <div v-show="pageActive">
    <h1> Login </h1>
    <br>
    <span> First IP    </span>
    <input style="background: #fcf5ed;" v-model="ipFirst">
    <br>
    <br>
    <span> Second IP    </span>
    <input style="background: #fcf5ed;" v-model="ipSecond">
    <br>
    <br>
    <span> Third IP    </span>
    <input style="background: #fcf5ed;" v-model="ipThird">
    <br>
    <br>
    <button style="background: #fcf5ed;" :disabled="isActive" type="button" @click="connect">submit</button>
    <br>
  </div>
  <div v-show="pageActive2" class="grid-container">
    <div class="item">
      <span>current state : {{state["P1C1"]}}</span>
      <br>
      <span> PI : 1</span>
      <br>
      <span> Cluster : 1 </span>
      <br>
        <v-switch
          v-model="toggle['P1C1']"
          hide-details
          true-value="yes"
          false-value="no"
          :label="`Switch: ${toggle['P1C1']}`"
          @click="toggleF('P1C1')"
        ></v-switch>
    </div>
    <div class="item">
      <span>current state : {{state["P2C1"]}}</span>
      <br>
      <span> PI : 2</span>
      <br>
      <span> Cluster : 1 </span>
      <br>
        <v-switch
          v-model="toggle['P2C1']"
          hide-details
          true-value="yes"
          false-value="no"
          :label="`Switch: ${toggle['P2C1']}`"
          @click="toggleF('P2C1')"
        ></v-switch>
    </div>
    <div class="item">
      <span>current state : {{state["P2C2"]}}</span>
      <br>
      <span> PI : 2</span>
      <br>
      <span> Cluster : 2 </span>
      <br>
        <v-switch
          v-model="toggle['P2C2']"
          hide-details
          true-value="yes"
          false-value="no"
          :label="`Switch: ${toggle['P2C2']}`"
          @click="toggleF('P2C2')"
        ></v-switch>
    </div>
    <div class="item">
      <span>current state : {{state["P3C2"]}}</span>
      <br>
      <span> PI : 3</span>
      <br>
      <span> Cluster : 2 </span>
      <br>
        <v-switch
          v-model="toggle['P3C2']"
          hide-details
          true-value="yes"
          false-value="no"
          :label="`Switch: ${toggle['P3C2']}`"
          @click="toggleF('P3C2')"
        ></v-switch>
    </div>
  </div>

</template>


<script>
    export default{
        name:'loginPage',
        components: {
        },
    data() {
        return{
            message : "temp",
            pageActive : true,
            pageActive2 : false,
            ipFirst : "192.168.1.147:8080",
            ipSecond : "192.168.1.150:8080",
            ipThird : "192.168.1.163:8080",
            show: "all",
            undoneTasks: 0,
            doneTasks: 0, 
            list: [],
            dict : {},
            state : {
              "P1C1":"P1C1",
              "P2C1":"P2C1",
              "P2C2":"P2C2",
              "P3C2":"P3C2"
            },
            connectSucceed : 0,
            toggle : {
              "P1C1":true,
              "P2C1":true,
              "P2C2":true,
              "P3C2":true
            },
            
            isActive : false
        }
    },
    methods: {
        toggleF(ID){
          if(this.toggle[ID])
            console.log("this.toggle[ID]");
        },
        connect(){
          // 建立 WebSocket (本例 server 端於本地運行)
          this.isActive =  true
          let url = 'ws://' + this.ipFirst // ws://localhost:3000
          var wsf = new WebSocket(url)
          wsf.onopen = () => {
            console.log('open connection')
            this.this.dict["ipFirst"] = url
            this.this.dict["ipWsf"] = wsf
            this.connectSucceed += 1
          }
          wsf.onclose = () => {
            console.log('close connection');
          }
          let urls = 'ws://' + this.ipSecond // ws://localhost:3000
          var wss = new WebSocket(urls)
          wss.onopen = () => {
            console.log('open connection')
            this.this.dict["ipSecond"] = urls
            this.this.dict["ipWss"] = wss
            this.connectSucceed += 1
          }
          wss.onclose = () => {
            console.log('close connection');
          }
          let urlt = 'ws://' + this.pThird // ws://localhost:3000
          var wst = new WebSocket(urlt)
          wst.onopen = () => {
            console.log('open connection')
            this.this.dict["ipThird"] = urlt
            this.this.dict["ipWst"] = wst
            this.connectSucceed += 1
          }
          wst.onclose = () => {
            console.log('close connection');
          }
          this.pageActive = false
          this.pageActive2 = true
          this.isActive =  false
        }
    },
    computed: {
    }
        
}


</script>


<style>
.grid-container {
  display: grid;
  grid-template-columns: repeat(2, 1fr);
  grid-template-rows: repeat(2, 1fr);
  grid-gap: 10px;
}

.item {
  background-color: #fcf5ed;
  padding: 20px;
}
</style>