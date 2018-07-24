.pragma library
.import Protobuf 1.0 as Protobuf
'use strict';

var _file = {
  get descriptor() {
    if(!this._desc) {
      this._desc = Protobuf.DescriptorPool.addFileDescriptor('ChBTdGF0dXNEYXRhLnByb3RvEgpmYW50YXN5Yml0IoYBCgxQbGF5ZXJTdGF0dXMSDgoGdGVhbWlkGBQgASgJEi8KBnN0YXR1cxgeIAEoDjIfLmZhbnRhc3liaXQuUGxheWVyU3RhdHVzLlN0YXR1cyI1CgZTdGF0dXMSCgoGQUNUSVZFEAASDAoISU5BQ1RJVkUQARIJCgVPVEhFUhADEgYKAkZBEAQimQEKCkdhbWVTdGF0dXMSLQoGc3RhdHVzGAogASgOMh0uZmFudGFzeWJpdC5HYW1lU3RhdHVzLlN0YXR1cxIQCghkYXRldGltZRgUIAEoDSJKCgZTdGF0dXMSDQoJU0NIRURVTEVEEAASCwoHUFJFR0FNRRACEgoKBklOR0FNRRADEgwKCFBPU1RHQU1FEAUSCgoGQ0xPU0VEEAQiRgoJVGVhbURlcHRoEgoKAnFiGAogAygJEgoKAnJiGBQgAygJEgoKAndyGB4gAygJEgoKAnRlGCggAygJEgkKAWsYMiADKAkifwoLR2xvYmFsU3RhdGUSLAoFc3RhdGUYASABKA4yHS5mYW50YXN5Yml0Lkdsb2JhbFN0YXRlLlN0YXRlEg4KBnNlYXNvbhgKIAEoDRIMCgR3ZWVrGBQgASgNIiQKBVN0YXRlEg0KCU9GRlNFQVNPThAKEgwKCElOU0VBU09OEB4qKwoQUGxheWVyR2FtZVN0YXR1cxIHCgNPVVQQABIGCgJJThABEgYKAk5BEAI=', 569);
    }
    if (!this._desc)
      console.warn('Failed to initialize: StatusData.proto');
    return this._desc;
  },
};


var PlayerGameStatus = {
  OUT: 0,
  IN: 1,
  NA: 2,

  toString: function(value) {
    switch(value) {
      case 0: return 'OUT';
      case 1: return 'IN';
      case 2: return 'NA';
    }
  },
};
var PlayerStatus = (function() {
  var type = function(values) {
    this._fields = new Array(2);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof PlayerStatus) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  
  var Status = {
    ACTIVE: 0,
    INACTIVE: 1,
    OTHER: 3,
    FA: 4,
  
    toString: function(value) {
      switch(value) {
        case 0: return 'ACTIVE';
        case 1: return 'INACTIVE';
        case 3: return 'OTHER';
        case 4: return 'FA';
      }
    },
  };
  type.Status = Status;
  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(0));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setTeamid(rawArray[0][0]);
      this.setStatus(rawArray[0][1]);
    }
  };

  type.prototype.getTeamid = function() {
    return typeof this._fields[0] == 'undefined' ? '' : this._fields[0];
  }
  type.prototype.setTeamid = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearTeamid = function() {
    this._fields[0] = '';
  };
  Object.defineProperty(type.prototype, 'teamid', {
    get: type.prototype.getTeamid,
    set: type.prototype.setTeamid,
  });

  type.prototype.getStatus = function() {
    return typeof this._fields[1] == 'undefined' ? PlayerStatus.Status.ACTIVE : this._fields[1];
  }
  type.prototype.setStatus = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearStatus = function() {
    this._fields[1] = PlayerStatus.Status.ACTIVE;
  };
  Object.defineProperty(type.prototype, 'status', {
    get: type.prototype.getStatus,
    set: type.prototype.setStatus,
  });

  return type;
})();

var GameStatus = (function() {
  var type = function(values) {
    this._fields = new Array(2);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof GameStatus) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  
  var Status = {
    SCHEDULED: 0,
    PREGAME: 2,
    INGAME: 3,
    POSTGAME: 5,
    CLOSED: 4,
  
    toString: function(value) {
      switch(value) {
        case 0: return 'SCHEDULED';
        case 2: return 'PREGAME';
        case 3: return 'INGAME';
        case 5: return 'POSTGAME';
        case 4: return 'CLOSED';
      }
    },
  };
  type.Status = Status;
  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(1));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setStatus(rawArray[0][0]);
      this.setDatetime(rawArray[0][1]);
    }
  };

  type.prototype.getStatus = function() {
    return typeof this._fields[0] == 'undefined' ? GameStatus.Status.SCHEDULED : this._fields[0];
  }
  type.prototype.setStatus = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearStatus = function() {
    this._fields[0] = GameStatus.Status.SCHEDULED;
  };
  Object.defineProperty(type.prototype, 'status', {
    get: type.prototype.getStatus,
    set: type.prototype.setStatus,
  });

  type.prototype.getDatetime = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setDatetime = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearDatetime = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'datetime', {
    get: type.prototype.getDatetime,
    set: type.prototype.setDatetime,
  });

  return type;
})();

var TeamDepth = (function() {
  var type = function(values) {
    this._fields = new Array(5);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
  this._fields[0] = new Array();
  if (values && values.qb && values.qb instanceof Array) {
    this.qb(values.qb);
  }
  this._fields[1] = new Array();
  if (values && values.rb && values.rb instanceof Array) {
    this.rb(values.rb);
  }
  this._fields[2] = new Array();
  if (values && values.wr && values.wr instanceof Array) {
    this.wr(values.wr);
  }
  this._fields[3] = new Array();
  if (values && values.te && values.te instanceof Array) {
    this.te(values.te);
  }
  this._fields[4] = new Array();
  if (values && values.k && values.k instanceof Array) {
    this.k(values.k);
  }
    if (values instanceof TeamDepth) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(2));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      if (rawArray[0][0] instanceof Array) {
        this.qb(rawArray[0][0]);
      }
      if (rawArray[0][1] instanceof Array) {
        this.rb(rawArray[0][1]);
      }
      if (rawArray[0][2] instanceof Array) {
        this.wr(rawArray[0][2]);
      }
      if (rawArray[0][3] instanceof Array) {
        this.te(rawArray[0][3]);
      }
      if (rawArray[0][4] instanceof Array) {
        this.k(rawArray[0][4]);
      }
    }
  };

    // Replacement setter
  type.prototype.setQb = function(values) {
    if (!(values instanceof Array)) {
      throw new TypeError();
    }
      this._fields[0] = values.slice();
  };
  // Single value setter
  type.prototype.setQbAt = function(index, value) {
    this._fields[0][index] = value;
  };
  // Getter
  type.prototype.getQbAt = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    if(this._fields[0].length < index) {
      throw new RangeError();
    }
    return this._fields[0][index];
  };
  type.prototype.getQbAsArray = function() {
    return this._fields[0].slice();
  };
  type.prototype.qb = function(indexOrValues, value) {
    if (typeof indexOrValues == 'undefined') {
      throw new TypeError('Not enough arguments');
    } else if (typeof indexOrValues == 'number') {
      if (typeof value == 'undefined') {
        return this.getQbAt(indexOrValues);
      } else {
        this.setQbAt(indexOrValues, value);
      }
    } else {
      this.setQb(indexOrValues);
    }
  };
  var qbCount = function() {
    return this._fields[0].length;
  };
  Object.defineProperties(type.prototype, {
    qbCount: { get: qbCount },
    qbSize: { get: qbCount },
    qbLength: { get: qbCount },
  });
  type.prototype.getQbCount = qbCount;
  type.prototype.getQbSize = qbCount;
  type.prototype.getQbLength = qbCount;
  type.prototype.addQb = function(value) {
    if (typeof value == 'undefined') {
      throw new TypeError('Cannot add undefined.');
    }
    this._fields[0].push(value);
  };
  type.prototype.resizeQb = function(len) {
    this._fields[0].length = values.length;
  };
  type.prototype.removeQb = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    this._fields[0].splice(index, 1);
  };
  type.prototype.clearQb = function() {
    this._fields[0].length = 0;
  };
    // Replacement setter
  type.prototype.setRb = function(values) {
    if (!(values instanceof Array)) {
      throw new TypeError();
    }
      this._fields[1] = values.slice();
  };
  // Single value setter
  type.prototype.setRbAt = function(index, value) {
    this._fields[1][index] = value;
  };
  // Getter
  type.prototype.getRbAt = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    if(this._fields[1].length < index) {
      throw new RangeError();
    }
    return this._fields[1][index];
  };
  type.prototype.getRbAsArray = function() {
    return this._fields[1].slice();
  };
  type.prototype.rb = function(indexOrValues, value) {
    if (typeof indexOrValues == 'undefined') {
      throw new TypeError('Not enough arguments');
    } else if (typeof indexOrValues == 'number') {
      if (typeof value == 'undefined') {
        return this.getRbAt(indexOrValues);
      } else {
        this.setRbAt(indexOrValues, value);
      }
    } else {
      this.setRb(indexOrValues);
    }
  };
  var rbCount = function() {
    return this._fields[1].length;
  };
  Object.defineProperties(type.prototype, {
    rbCount: { get: rbCount },
    rbSize: { get: rbCount },
    rbLength: { get: rbCount },
  });
  type.prototype.getRbCount = rbCount;
  type.prototype.getRbSize = rbCount;
  type.prototype.getRbLength = rbCount;
  type.prototype.addRb = function(value) {
    if (typeof value == 'undefined') {
      throw new TypeError('Cannot add undefined.');
    }
    this._fields[1].push(value);
  };
  type.prototype.resizeRb = function(len) {
    this._fields[1].length = values.length;
  };
  type.prototype.removeRb = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    this._fields[1].splice(index, 1);
  };
  type.prototype.clearRb = function() {
    this._fields[1].length = 0;
  };
    // Replacement setter
  type.prototype.setWr = function(values) {
    if (!(values instanceof Array)) {
      throw new TypeError();
    }
      this._fields[2] = values.slice();
  };
  // Single value setter
  type.prototype.setWrAt = function(index, value) {
    this._fields[2][index] = value;
  };
  // Getter
  type.prototype.getWrAt = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    if(this._fields[2].length < index) {
      throw new RangeError();
    }
    return this._fields[2][index];
  };
  type.prototype.getWrAsArray = function() {
    return this._fields[2].slice();
  };
  type.prototype.wr = function(indexOrValues, value) {
    if (typeof indexOrValues == 'undefined') {
      throw new TypeError('Not enough arguments');
    } else if (typeof indexOrValues == 'number') {
      if (typeof value == 'undefined') {
        return this.getWrAt(indexOrValues);
      } else {
        this.setWrAt(indexOrValues, value);
      }
    } else {
      this.setWr(indexOrValues);
    }
  };
  var wrCount = function() {
    return this._fields[2].length;
  };
  Object.defineProperties(type.prototype, {
    wrCount: { get: wrCount },
    wrSize: { get: wrCount },
    wrLength: { get: wrCount },
  });
  type.prototype.getWrCount = wrCount;
  type.prototype.getWrSize = wrCount;
  type.prototype.getWrLength = wrCount;
  type.prototype.addWr = function(value) {
    if (typeof value == 'undefined') {
      throw new TypeError('Cannot add undefined.');
    }
    this._fields[2].push(value);
  };
  type.prototype.resizeWr = function(len) {
    this._fields[2].length = values.length;
  };
  type.prototype.removeWr = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    this._fields[2].splice(index, 1);
  };
  type.prototype.clearWr = function() {
    this._fields[2].length = 0;
  };
    // Replacement setter
  type.prototype.setTe = function(values) {
    if (!(values instanceof Array)) {
      throw new TypeError();
    }
      this._fields[3] = values.slice();
  };
  // Single value setter
  type.prototype.setTeAt = function(index, value) {
    this._fields[3][index] = value;
  };
  // Getter
  type.prototype.getTeAt = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    if(this._fields[3].length < index) {
      throw new RangeError();
    }
    return this._fields[3][index];
  };
  type.prototype.getTeAsArray = function() {
    return this._fields[3].slice();
  };
  type.prototype.te = function(indexOrValues, value) {
    if (typeof indexOrValues == 'undefined') {
      throw new TypeError('Not enough arguments');
    } else if (typeof indexOrValues == 'number') {
      if (typeof value == 'undefined') {
        return this.getTeAt(indexOrValues);
      } else {
        this.setTeAt(indexOrValues, value);
      }
    } else {
      this.setTe(indexOrValues);
    }
  };
  var teCount = function() {
    return this._fields[3].length;
  };
  Object.defineProperties(type.prototype, {
    teCount: { get: teCount },
    teSize: { get: teCount },
    teLength: { get: teCount },
  });
  type.prototype.getTeCount = teCount;
  type.prototype.getTeSize = teCount;
  type.prototype.getTeLength = teCount;
  type.prototype.addTe = function(value) {
    if (typeof value == 'undefined') {
      throw new TypeError('Cannot add undefined.');
    }
    this._fields[3].push(value);
  };
  type.prototype.resizeTe = function(len) {
    this._fields[3].length = values.length;
  };
  type.prototype.removeTe = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    this._fields[3].splice(index, 1);
  };
  type.prototype.clearTe = function() {
    this._fields[3].length = 0;
  };
    // Replacement setter
  type.prototype.setK = function(values) {
    if (!(values instanceof Array)) {
      throw new TypeError();
    }
      this._fields[4] = values.slice();
  };
  // Single value setter
  type.prototype.setKAt = function(index, value) {
    this._fields[4][index] = value;
  };
  // Getter
  type.prototype.getKAt = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    if(this._fields[4].length < index) {
      throw new RangeError();
    }
    return this._fields[4][index];
  };
  type.prototype.getKAsArray = function() {
    return this._fields[4].slice();
  };
  type.prototype.k = function(indexOrValues, value) {
    if (typeof indexOrValues == 'undefined') {
      throw new TypeError('Not enough arguments');
    } else if (typeof indexOrValues == 'number') {
      if (typeof value == 'undefined') {
        return this.getKAt(indexOrValues);
      } else {
        this.setKAt(indexOrValues, value);
      }
    } else {
      this.setK(indexOrValues);
    }
  };
  var kCount = function() {
    return this._fields[4].length;
  };
  Object.defineProperties(type.prototype, {
    kCount: { get: kCount },
    kSize: { get: kCount },
    kLength: { get: kCount },
  });
  type.prototype.getKCount = kCount;
  type.prototype.getKSize = kCount;
  type.prototype.getKLength = kCount;
  type.prototype.addK = function(value) {
    if (typeof value == 'undefined') {
      throw new TypeError('Cannot add undefined.');
    }
    this._fields[4].push(value);
  };
  type.prototype.resizeK = function(len) {
    this._fields[4].length = values.length;
  };
  type.prototype.removeK = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    this._fields[4].splice(index, 1);
  };
  type.prototype.clearK = function() {
    this._fields[4].length = 0;
  };
  return type;
})();

var GlobalState = (function() {
  var type = function(values) {
    this._fields = new Array(3);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof GlobalState) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  
  var State = {
    OFFSEASON: 10,
    INSEASON: 30,
  
    toString: function(value) {
      switch(value) {
        case 10: return 'OFFSEASON';
        case 30: return 'INSEASON';
      }
    },
  };
  type.State = State;
  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(3));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setState(rawArray[0][0]);
      this.setSeason(rawArray[0][1]);
      this.setWeek(rawArray[0][2]);
    }
  };

  type.prototype.getState = function() {
    return typeof this._fields[0] == 'undefined' ? GlobalState.State.OFFSEASON : this._fields[0];
  }
  type.prototype.setState = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearState = function() {
    this._fields[0] = GlobalState.State.OFFSEASON;
  };
  Object.defineProperty(type.prototype, 'state', {
    get: type.prototype.getState,
    set: type.prototype.setState,
  });

  type.prototype.getSeason = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setSeason = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearSeason = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'season', {
    get: type.prototype.getSeason,
    set: type.prototype.setSeason,
  });

  type.prototype.getWeek = function() {
    return typeof this._fields[2] == 'undefined' ? 0 : this._fields[2];
  }
  type.prototype.setWeek = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearWeek = function() {
    this._fields[2] = 0;
  };
  Object.defineProperty(type.prototype, 'week', {
    get: type.prototype.getWeek,
    set: type.prototype.setWeek,
  });

  return type;
})();

