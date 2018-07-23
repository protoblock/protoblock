.pragma library
.import Protobuf 1.0 as Protobuf
'use strict';

var _file = {
  get descriptor() {
    if(!this._desc) {
      this._desc = Protobuf.DescriptorPool.addFileDescriptor('CgxFeERhdGEucHJvdG8SCmZhbnRhc3liaXQiMwoJU2V0dGxlUG9zEgoKAnBrGAogASgJEgsKA3F0eRgUIAEoBRINCgVwcmljZRgZIAEoBSImCghTdG9yZVBvcxILCgNxdHkYFCABKAUSDQoFcHJpY2UYGSABKAUiRQoHQm9va1BvcxIQCghwbGF5ZXJpZBgeIAEoCRIoCglwb3NpdGlvbnMYKCADKAsyFS5mYW50YXN5Yml0LlNldHRsZVBvcyJlCg1HYW1lU2V0dGxlUG9zEg4KBmdhbWVpZBgKIAEoCRIhCgRob21lGBQgAygLMhMuZmFudGFzeWJpdC5Cb29rUG9zEiEKBGF3YXkYHiADKAsyEy5mYW50YXN5Yml0LkJvb2tQb3MiOQoJT3JkZXJDb3JlEg8KB2J1eXNpZGUYCiABKAgSDAoEc2l6ZRgUIAEoBRINCgVwcmljZRgeIAEoBSI8CgVPcmRlchIOCgZyZWZudW0YCiABKAUSIwoEY29yZRgUIAEoCzIVLmZhbnRhc3liaXQuT3JkZXJDb3JlImQKCU9yZGVyRmlsbBIfCgRsZWZ0GAogASgLMhEuZmFudGFzeWJpdC5PcmRlchIRCglmaWxscHJpY2UYFCABKAUSEAoIZmlsbHNpemUYHiABKAUSEQoJdGltZXN0YW1wGCggASgFIowBCgxNYXJrZXRUaWNrZXISKwoEdHlwZRgBIAEoDjIdLmZhbnRhc3liaXQuTWFya2V0VGlja2VyLlR5cGUSDgoGc3ltYm9sGAIgASgJEgwKBHNpemUYFCABKAUSDQoFcHJpY2UYHiABKAUiIgoEVHlwZRIHCgNCSUQQARIHCgNBU0sQAhIICgRMQVNUEAMicwoIVHJhZGVUaWMSDgoGc3ltYm9sGAIgASgJEgwKBHNpemUYFCABKAUSDQoFcHJpY2UYHiABKAUSDgoGaXNoaWdoGCggASgFEg0KBWlzbG93GC0gASgFEg4KBmNoYW5nZRgyIAEoBRILCgN0aWMYPCABKAUiTAoORGVwdGhGZWVkRGVsdGESDgoGc3ltYm9sGAEgASgJEg0KBWlzYmlkGAogASgIEgwKBHNpemUYFCABKAUSDQoFcHJpY2UYHiABKAUidgoMQ29udHJhY3RPSExDEg4KBnN5bWJvbBgKIAEoCRIMCgRvcGVuGBQgASgFEgwKBGhpZ2gYHiABKAUSCwoDbG93GCggASgFEg0KBWNsb3NlGDIgASgFEg4KBnZvbHVtZRg8IAEoBRIOCgZjaGFuZ2UYRiABKAUiSAoJRGVwdGhJdGVtEg0KBWxldmVsGAogASgFEgkKAWIYHiABKAUSCQoBYRgoIAEoBRIKCgJicxgyIAEoBRIKCgJhcxg8IAEoBSKQAgoJQm9va0RlbHRhEhQKDGZhbnRhc3lfbmFtZRgKIAEoCRIOCgZzZXFudW0YFCABKAUSEAoIcGxheWVyaWQYHiABKAkSJQoGbmV3bmV3GCggASgLMhUuZmFudGFzeWJpdC5PcmRlckNvcmUSJgoEb2hsYxgpIAEoCzIYLmZhbnRhc3liaXQuQ29udHJhY3RPSExDEiIKB3JlbW92ZXMYMiADKAsyES5mYW50YXN5Yml0Lk9yZGVyEisKCWxldmVsMnRpYxg8IAMoCzIYLmZhbnRhc3liaXQuTWFya2V0VGlja2VyEisKCWxldmVsMXRpYxhGIAMoCzIYLmZhbnRhc3liaXQuTWFya2V0VGlja2VyIl8KC01hcmtldFF1b3RlEgoKAmJzGAogASgFEgkKAWIYFCABKAUSCQoBYRgeIAEoBRIKCgJhcxgoIAEoBRIJCgFsGDIgASgFEgoKAmxzGDwgASgFEgsKA3VkbhhGIAEoBSKkAQoOTWFya2V0U25hcHNob3QSDgoGc3ltYm9sGAogASgJEgwKBHdlZWsYCyABKAUSJAoFZGVwdGgYRiADKAsyFS5mYW50YXN5Yml0LkRlcHRoSXRlbRImCgRvaGxjGBQgASgLMhguZmFudGFzeWJpdC5Db250cmFjdE9ITEMSJgoFcXVvdGUYHiABKAsyFy5mYW50YXN5Yml0Lk1hcmtldFF1b3Rl', 1587);
    }
    if (!this._desc)
      console.warn('Failed to initialize: ExData.proto');
    return this._desc;
  },
};

var SettlePos = (function() {
  var type = function(values) {
    this._fields = new Array(3);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof SettlePos) {
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

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(0));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setPk(rawArray[0][0]);
      this.setQty(rawArray[0][1]);
      this.setPrice(rawArray[0][2]);
    }
  };

  type.prototype.getPk = function() {
    return typeof this._fields[0] == 'undefined' ? '' : this._fields[0];
  }
  type.prototype.setPk = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearPk = function() {
    this._fields[0] = '';
  };
  Object.defineProperty(type.prototype, 'pk', {
    get: type.prototype.getPk,
    set: type.prototype.setPk,
  });

  type.prototype.getQty = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setQty = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearQty = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'qty', {
    get: type.prototype.getQty,
    set: type.prototype.setQty,
  });

  type.prototype.getPrice = function() {
    return typeof this._fields[2] == 'undefined' ? 0 : this._fields[2];
  }
  type.prototype.setPrice = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearPrice = function() {
    this._fields[2] = 0;
  };
  Object.defineProperty(type.prototype, 'price', {
    get: type.prototype.getPrice,
    set: type.prototype.setPrice,
  });

  return type;
})();

var StorePos = (function() {
  var type = function(values) {
    this._fields = new Array(2);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof StorePos) {
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

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(1));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setQty(rawArray[0][0]);
      this.setPrice(rawArray[0][1]);
    }
  };

  type.prototype.getQty = function() {
    return typeof this._fields[0] == 'undefined' ? 0 : this._fields[0];
  }
  type.prototype.setQty = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearQty = function() {
    this._fields[0] = 0;
  };
  Object.defineProperty(type.prototype, 'qty', {
    get: type.prototype.getQty,
    set: type.prototype.setQty,
  });

  type.prototype.getPrice = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setPrice = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearPrice = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'price', {
    get: type.prototype.getPrice,
    set: type.prototype.setPrice,
  });

  return type;
})();

var BookPos = (function() {
  var type = function(values) {
    this._fields = new Array(2);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
  this._positions = new Array();
  this._fields[1] = new Array();
  if (values && values.positions && values.positions instanceof Array) {
    this.positions(values.positions);
  }
    if (values instanceof BookPos) {
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
      this.setPlayerid(rawArray[0][0]);
      field = rawArray[0][1];
      if (field instanceof Array) {
        for (var i = 0; i < field.length; ++i) {
          if (typeof this._positions[i] == 'undefined') {
            var msg = new SettlePos();
            this._positions[i] = msg;
            this._fields[1][i] = msg._raw;
          }
          this._positions[i]._mergeFromRawArray(field[i]);
        }
      }
    }
  };

  type.prototype.getPlayerid = function() {
    return typeof this._fields[0] == 'undefined' ? '' : this._fields[0];
  }
  type.prototype.setPlayerid = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearPlayerid = function() {
    this._fields[0] = '';
  };
  Object.defineProperty(type.prototype, 'playerid', {
    get: type.prototype.getPlayerid,
    set: type.prototype.setPlayerid,
  });

    // Replacement setter
  type.prototype.setPositions = function(values) {
    if (!(values instanceof Array)) {
      throw new TypeError();
    }
    this._fields[1].length = values.length;
     this._positions.length = values.length;
     for (var i = 0; i < values.length; ++i) {
       var msg = new SettlePos(values[i]);
       this._positions[i] = msg;
       this._fields[1][i] = msg._raw;
     }
  };
  // Single value setter
  type.prototype.setPositionsAt = function(index, value) {
    var msg = this._maybeConvertToMessage(SettlePos, value);
    this._positions[index] = msg;
    this._fields[1][index] = msg._raw;
  };
  // Getter
  type.prototype.getPositionsAt = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    if(this._fields[1].length < index) {
      throw new RangeError();
    }
    return this._positions[index];
  };
  type.prototype.getPositionsAsArray = function() {
    return this._positions.slice();
  };
  type.prototype.positions = function(indexOrValues, value) {
    if (typeof indexOrValues == 'undefined') {
      throw new TypeError('Not enough arguments');
    } else if (typeof indexOrValues == 'number') {
      if (typeof value == 'undefined') {
        return this.getPositionsAt(indexOrValues);
      } else {
        this.setPositionsAt(indexOrValues, value);
      }
    } else {
      this.setPositions(indexOrValues);
    }
  };
  var positionsCount = function() {
  console.assert(this._positions.length == this._fields[1].length);
    return this._positions.length;
  };
  Object.defineProperties(type.prototype, {
    positionsCount: { get: positionsCount },
    positionsSize: { get: positionsCount },
    positionsLength: { get: positionsCount },
  });
  type.prototype.getPositionsCount = positionsCount;
  type.prototype.getPositionsSize = positionsCount;
  type.prototype.getPositionsLength = positionsCount;
  type.prototype.addPositions = function(value) {
    if (typeof value == 'undefined') {
      throw new TypeError('Cannot add undefined.');
    }
    var msg = this._maybeConvertToMessage(SettlePos, value);
    this._positions.push(msg);
    this._fields[1].push(msg._raw);
  console.assert(this._positions.length == this._fields[1].length);
  };
  type.prototype.removePositions = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    this._fields[1].splice(index, 1);
    this._positions.splice(index, 1);
  console.assert(this._positions.length == this._fields[1].length);
  };
  type.prototype.clearPositions = function() {
    this._fields[1].length = 0;
    this._positions.length = 0;
  console.assert(this._positions.length == this._fields[1].length);
  };
  return type;
})();

var GameSettlePos = (function() {
  var type = function(values) {
    this._fields = new Array(3);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
  this._home = new Array();
  this._fields[1] = new Array();
  if (values && values.home && values.home instanceof Array) {
    this.home(values.home);
  }
  this._away = new Array();
  this._fields[2] = new Array();
  if (values && values.away && values.away instanceof Array) {
    this.away(values.away);
  }
    if (values instanceof GameSettlePos) {
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

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(3));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setGameid(rawArray[0][0]);
      field = rawArray[0][1];
      if (field instanceof Array) {
        for (var i = 0; i < field.length; ++i) {
          if (typeof this._home[i] == 'undefined') {
            var msg = new BookPos();
            this._home[i] = msg;
            this._fields[1][i] = msg._raw;
          }
          this._home[i]._mergeFromRawArray(field[i]);
        }
      }
      field = rawArray[0][2];
      if (field instanceof Array) {
        for (var i = 0; i < field.length; ++i) {
          if (typeof this._away[i] == 'undefined') {
            var msg = new BookPos();
            this._away[i] = msg;
            this._fields[2][i] = msg._raw;
          }
          this._away[i]._mergeFromRawArray(field[i]);
        }
      }
    }
  };

  type.prototype.getGameid = function() {
    return typeof this._fields[0] == 'undefined' ? '' : this._fields[0];
  }
  type.prototype.setGameid = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearGameid = function() {
    this._fields[0] = '';
  };
  Object.defineProperty(type.prototype, 'gameid', {
    get: type.prototype.getGameid,
    set: type.prototype.setGameid,
  });

    // Replacement setter
  type.prototype.setHome = function(values) {
    if (!(values instanceof Array)) {
      throw new TypeError();
    }
    this._fields[1].length = values.length;
     this._home.length = values.length;
     for (var i = 0; i < values.length; ++i) {
       var msg = new BookPos(values[i]);
       this._home[i] = msg;
       this._fields[1][i] = msg._raw;
     }
  };
  // Single value setter
  type.prototype.setHomeAt = function(index, value) {
    var msg = this._maybeConvertToMessage(BookPos, value);
    this._home[index] = msg;
    this._fields[1][index] = msg._raw;
  };
  // Getter
  type.prototype.getHomeAt = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    if(this._fields[1].length < index) {
      throw new RangeError();
    }
    return this._home[index];
  };
  type.prototype.getHomeAsArray = function() {
    return this._home.slice();
  };
  type.prototype.home = function(indexOrValues, value) {
    if (typeof indexOrValues == 'undefined') {
      throw new TypeError('Not enough arguments');
    } else if (typeof indexOrValues == 'number') {
      if (typeof value == 'undefined') {
        return this.getHomeAt(indexOrValues);
      } else {
        this.setHomeAt(indexOrValues, value);
      }
    } else {
      this.setHome(indexOrValues);
    }
  };
  var homeCount = function() {
  console.assert(this._home.length == this._fields[1].length);
    return this._home.length;
  };
  Object.defineProperties(type.prototype, {
    homeCount: { get: homeCount },
    homeSize: { get: homeCount },
    homeLength: { get: homeCount },
  });
  type.prototype.getHomeCount = homeCount;
  type.prototype.getHomeSize = homeCount;
  type.prototype.getHomeLength = homeCount;
  type.prototype.addHome = function(value) {
    if (typeof value == 'undefined') {
      throw new TypeError('Cannot add undefined.');
    }
    var msg = this._maybeConvertToMessage(BookPos, value);
    this._home.push(msg);
    this._fields[1].push(msg._raw);
  console.assert(this._home.length == this._fields[1].length);
  };
  type.prototype.removeHome = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    this._fields[1].splice(index, 1);
    this._home.splice(index, 1);
  console.assert(this._home.length == this._fields[1].length);
  };
  type.prototype.clearHome = function() {
    this._fields[1].length = 0;
    this._home.length = 0;
  console.assert(this._home.length == this._fields[1].length);
  };
    // Replacement setter
  type.prototype.setAway = function(values) {
    if (!(values instanceof Array)) {
      throw new TypeError();
    }
    this._fields[2].length = values.length;
     this._away.length = values.length;
     for (var i = 0; i < values.length; ++i) {
       var msg = new BookPos(values[i]);
       this._away[i] = msg;
       this._fields[2][i] = msg._raw;
     }
  };
  // Single value setter
  type.prototype.setAwayAt = function(index, value) {
    var msg = this._maybeConvertToMessage(BookPos, value);
    this._away[index] = msg;
    this._fields[2][index] = msg._raw;
  };
  // Getter
  type.prototype.getAwayAt = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    if(this._fields[2].length < index) {
      throw new RangeError();
    }
    return this._away[index];
  };
  type.prototype.getAwayAsArray = function() {
    return this._away.slice();
  };
  type.prototype.away = function(indexOrValues, value) {
    if (typeof indexOrValues == 'undefined') {
      throw new TypeError('Not enough arguments');
    } else if (typeof indexOrValues == 'number') {
      if (typeof value == 'undefined') {
        return this.getAwayAt(indexOrValues);
      } else {
        this.setAwayAt(indexOrValues, value);
      }
    } else {
      this.setAway(indexOrValues);
    }
  };
  var awayCount = function() {
  console.assert(this._away.length == this._fields[2].length);
    return this._away.length;
  };
  Object.defineProperties(type.prototype, {
    awayCount: { get: awayCount },
    awaySize: { get: awayCount },
    awayLength: { get: awayCount },
  });
  type.prototype.getAwayCount = awayCount;
  type.prototype.getAwaySize = awayCount;
  type.prototype.getAwayLength = awayCount;
  type.prototype.addAway = function(value) {
    if (typeof value == 'undefined') {
      throw new TypeError('Cannot add undefined.');
    }
    var msg = this._maybeConvertToMessage(BookPos, value);
    this._away.push(msg);
    this._fields[2].push(msg._raw);
  console.assert(this._away.length == this._fields[2].length);
  };
  type.prototype.removeAway = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    this._fields[2].splice(index, 1);
    this._away.splice(index, 1);
  console.assert(this._away.length == this._fields[2].length);
  };
  type.prototype.clearAway = function() {
    this._fields[2].length = 0;
    this._away.length = 0;
  console.assert(this._away.length == this._fields[2].length);
  };
  return type;
})();

var OrderCore = (function() {
  var type = function(values) {
    this._fields = new Array(3);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof OrderCore) {
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

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(4));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setBuyside(rawArray[0][0]);
      this.setSize(rawArray[0][1]);
      this.setPrice(rawArray[0][2]);
    }
  };

  type.prototype.getBuyside = function() {
    return typeof this._fields[0] == 'undefined' ? false : this._fields[0];
  }
  type.prototype.setBuyside = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearBuyside = function() {
    this._fields[0] = false;
  };
  Object.defineProperty(type.prototype, 'buyside', {
    get: type.prototype.getBuyside,
    set: type.prototype.setBuyside,
  });

  type.prototype.getSize = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setSize = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearSize = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'size', {
    get: type.prototype.getSize,
    set: type.prototype.setSize,
  });

  type.prototype.getPrice = function() {
    return typeof this._fields[2] == 'undefined' ? 0 : this._fields[2];
  }
  type.prototype.setPrice = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearPrice = function() {
    this._fields[2] = 0;
  };
  Object.defineProperty(type.prototype, 'price', {
    get: type.prototype.getPrice,
    set: type.prototype.setPrice,
  });

  return type;
})();

var Order = (function() {
  var type = function(values) {
    this._fields = new Array(2);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof Order) {
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

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(5));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setRefnum(rawArray[0][0]);
      if (typeof this.core == 'undefined') {
        this.setCore({});
      }
      this._core._mergeFromRawArray(rawArray[0][1]);
    }
  };

  type.prototype.getRefnum = function() {
    return typeof this._fields[0] == 'undefined' ? 0 : this._fields[0];
  }
  type.prototype.setRefnum = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearRefnum = function() {
    this._fields[0] = 0;
  };
  Object.defineProperty(type.prototype, 'refnum', {
    get: type.prototype.getRefnum,
    set: type.prototype.setRefnum,
  });

  type.prototype.getCore = function() {
    return this._core;
  }
  type.prototype.setCore = function(value) {
    var msg = this._maybeConvertToMessage(OrderCore, value);
    this._core = msg;
    this._fields[1] = msg._raw;
  };
  type.prototype.clearCore = function() {
    this._fields[1] = undefined;
    this._core = undefined;
  };
  Object.defineProperty(type.prototype, 'core', {
    get: type.prototype.getCore,
    set: type.prototype.setCore,
  });

  return type;
})();

var OrderFill = (function() {
  var type = function(values) {
    this._fields = new Array(4);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof OrderFill) {
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

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(6));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      if (typeof this.left == 'undefined') {
        this.setLeft({});
      }
      this._left._mergeFromRawArray(rawArray[0][0]);
      this.setFillprice(rawArray[0][1]);
      this.setFillsize(rawArray[0][2]);
      this.setTimestamp(rawArray[0][3]);
    }
  };

  type.prototype.getLeft = function() {
    return this._left;
  }
  type.prototype.setLeft = function(value) {
    var msg = this._maybeConvertToMessage(Order, value);
    this._left = msg;
    this._fields[0] = msg._raw;
  };
  type.prototype.clearLeft = function() {
    this._fields[0] = undefined;
    this._left = undefined;
  };
  Object.defineProperty(type.prototype, 'left', {
    get: type.prototype.getLeft,
    set: type.prototype.setLeft,
  });

  type.prototype.getFillprice = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setFillprice = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearFillprice = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'fillprice', {
    get: type.prototype.getFillprice,
    set: type.prototype.setFillprice,
  });

  type.prototype.getFillsize = function() {
    return typeof this._fields[2] == 'undefined' ? 0 : this._fields[2];
  }
  type.prototype.setFillsize = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearFillsize = function() {
    this._fields[2] = 0;
  };
  Object.defineProperty(type.prototype, 'fillsize', {
    get: type.prototype.getFillsize,
    set: type.prototype.setFillsize,
  });

  type.prototype.getTimestamp = function() {
    return typeof this._fields[3] == 'undefined' ? 0 : this._fields[3];
  }
  type.prototype.setTimestamp = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearTimestamp = function() {
    this._fields[3] = 0;
  };
  Object.defineProperty(type.prototype, 'timestamp', {
    get: type.prototype.getTimestamp,
    set: type.prototype.setTimestamp,
  });

  return type;
})();

var MarketTicker = (function() {
  var type = function(values) {
    this._fields = new Array(4);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof MarketTicker) {
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

  
  var Type = {
    BID: 1,
    ASK: 2,
    LAST: 3,
  
    toString: function(value) {
      switch(value) {
        case 1: return 'BID';
        case 2: return 'ASK';
        case 3: return 'LAST';
      }
    },
  };
  type.Type = Type;
  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(7));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setType(rawArray[0][0]);
      this.setSymbol(rawArray[0][1]);
      this.setSize(rawArray[0][2]);
      this.setPrice(rawArray[0][3]);
    }
  };

  type.prototype.getType = function() {
    return typeof this._fields[0] == 'undefined' ? MarketTicker.Type.BID : this._fields[0];
  }
  type.prototype.setType = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearType = function() {
    this._fields[0] = MarketTicker.Type.BID;
  };
  Object.defineProperty(type.prototype, 'type', {
    get: type.prototype.getType,
    set: type.prototype.setType,
  });

  type.prototype.getSymbol = function() {
    return typeof this._fields[1] == 'undefined' ? '' : this._fields[1];
  }
  type.prototype.setSymbol = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearSymbol = function() {
    this._fields[1] = '';
  };
  Object.defineProperty(type.prototype, 'symbol', {
    get: type.prototype.getSymbol,
    set: type.prototype.setSymbol,
  });

  type.prototype.getSize = function() {
    return typeof this._fields[2] == 'undefined' ? 0 : this._fields[2];
  }
  type.prototype.setSize = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearSize = function() {
    this._fields[2] = 0;
  };
  Object.defineProperty(type.prototype, 'size', {
    get: type.prototype.getSize,
    set: type.prototype.setSize,
  });

  type.prototype.getPrice = function() {
    return typeof this._fields[3] == 'undefined' ? 0 : this._fields[3];
  }
  type.prototype.setPrice = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearPrice = function() {
    this._fields[3] = 0;
  };
  Object.defineProperty(type.prototype, 'price', {
    get: type.prototype.getPrice,
    set: type.prototype.setPrice,
  });

  return type;
})();

var TradeTic = (function() {
  var type = function(values) {
    this._fields = new Array(7);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof TradeTic) {
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

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(8));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setSymbol(rawArray[0][0]);
      this.setSize(rawArray[0][1]);
      this.setPrice(rawArray[0][2]);
      this.setIshigh(rawArray[0][3]);
      this.setIslow(rawArray[0][4]);
      this.setChange(rawArray[0][5]);
      this.setTic(rawArray[0][6]);
    }
  };

  type.prototype.getSymbol = function() {
    return typeof this._fields[0] == 'undefined' ? '' : this._fields[0];
  }
  type.prototype.setSymbol = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearSymbol = function() {
    this._fields[0] = '';
  };
  Object.defineProperty(type.prototype, 'symbol', {
    get: type.prototype.getSymbol,
    set: type.prototype.setSymbol,
  });

  type.prototype.getSize = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setSize = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearSize = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'size', {
    get: type.prototype.getSize,
    set: type.prototype.setSize,
  });

  type.prototype.getPrice = function() {
    return typeof this._fields[2] == 'undefined' ? 0 : this._fields[2];
  }
  type.prototype.setPrice = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearPrice = function() {
    this._fields[2] = 0;
  };
  Object.defineProperty(type.prototype, 'price', {
    get: type.prototype.getPrice,
    set: type.prototype.setPrice,
  });

  type.prototype.getIshigh = function() {
    return typeof this._fields[3] == 'undefined' ? 0 : this._fields[3];
  }
  type.prototype.setIshigh = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearIshigh = function() {
    this._fields[3] = 0;
  };
  Object.defineProperty(type.prototype, 'ishigh', {
    get: type.prototype.getIshigh,
    set: type.prototype.setIshigh,
  });

  type.prototype.getIslow = function() {
    return typeof this._fields[4] == 'undefined' ? 0 : this._fields[4];
  }
  type.prototype.setIslow = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearIslow = function() {
    this._fields[4] = 0;
  };
  Object.defineProperty(type.prototype, 'islow', {
    get: type.prototype.getIslow,
    set: type.prototype.setIslow,
  });

  type.prototype.getChange = function() {
    return typeof this._fields[5] == 'undefined' ? 0 : this._fields[5];
  }
  type.prototype.setChange = function(value) {
      this._fields[5] = value;
  };
  type.prototype.clearChange = function() {
    this._fields[5] = 0;
  };
  Object.defineProperty(type.prototype, 'change', {
    get: type.prototype.getChange,
    set: type.prototype.setChange,
  });

  type.prototype.getTic = function() {
    return typeof this._fields[6] == 'undefined' ? 0 : this._fields[6];
  }
  type.prototype.setTic = function(value) {
      this._fields[6] = value;
  };
  type.prototype.clearTic = function() {
    this._fields[6] = 0;
  };
  Object.defineProperty(type.prototype, 'tic', {
    get: type.prototype.getTic,
    set: type.prototype.setTic,
  });

  return type;
})();

var DepthFeedDelta = (function() {
  var type = function(values) {
    this._fields = new Array(4);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof DepthFeedDelta) {
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

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(9));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setSymbol(rawArray[0][0]);
      this.setIsbid(rawArray[0][1]);
      this.setSize(rawArray[0][2]);
      this.setPrice(rawArray[0][3]);
    }
  };

  type.prototype.getSymbol = function() {
    return typeof this._fields[0] == 'undefined' ? '' : this._fields[0];
  }
  type.prototype.setSymbol = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearSymbol = function() {
    this._fields[0] = '';
  };
  Object.defineProperty(type.prototype, 'symbol', {
    get: type.prototype.getSymbol,
    set: type.prototype.setSymbol,
  });

  type.prototype.getIsbid = function() {
    return typeof this._fields[1] == 'undefined' ? false : this._fields[1];
  }
  type.prototype.setIsbid = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearIsbid = function() {
    this._fields[1] = false;
  };
  Object.defineProperty(type.prototype, 'isbid', {
    get: type.prototype.getIsbid,
    set: type.prototype.setIsbid,
  });

  type.prototype.getSize = function() {
    return typeof this._fields[2] == 'undefined' ? 0 : this._fields[2];
  }
  type.prototype.setSize = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearSize = function() {
    this._fields[2] = 0;
  };
  Object.defineProperty(type.prototype, 'size', {
    get: type.prototype.getSize,
    set: type.prototype.setSize,
  });

  type.prototype.getPrice = function() {
    return typeof this._fields[3] == 'undefined' ? 0 : this._fields[3];
  }
  type.prototype.setPrice = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearPrice = function() {
    this._fields[3] = 0;
  };
  Object.defineProperty(type.prototype, 'price', {
    get: type.prototype.getPrice,
    set: type.prototype.setPrice,
  });

  return type;
})();

var ContractOHLC = (function() {
  var type = function(values) {
    this._fields = new Array(7);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof ContractOHLC) {
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

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(10));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setSymbol(rawArray[0][0]);
      this.setOpen(rawArray[0][1]);
      this.setHigh(rawArray[0][2]);
      this.setLow(rawArray[0][3]);
      this.setClose(rawArray[0][4]);
      this.setVolume(rawArray[0][5]);
      this.setChange(rawArray[0][6]);
    }
  };

  type.prototype.getSymbol = function() {
    return typeof this._fields[0] == 'undefined' ? '' : this._fields[0];
  }
  type.prototype.setSymbol = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearSymbol = function() {
    this._fields[0] = '';
  };
  Object.defineProperty(type.prototype, 'symbol', {
    get: type.prototype.getSymbol,
    set: type.prototype.setSymbol,
  });

  type.prototype.getOpen = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setOpen = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearOpen = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'open', {
    get: type.prototype.getOpen,
    set: type.prototype.setOpen,
  });

  type.prototype.getHigh = function() {
    return typeof this._fields[2] == 'undefined' ? 0 : this._fields[2];
  }
  type.prototype.setHigh = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearHigh = function() {
    this._fields[2] = 0;
  };
  Object.defineProperty(type.prototype, 'high', {
    get: type.prototype.getHigh,
    set: type.prototype.setHigh,
  });

  type.prototype.getLow = function() {
    return typeof this._fields[3] == 'undefined' ? 0 : this._fields[3];
  }
  type.prototype.setLow = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearLow = function() {
    this._fields[3] = 0;
  };
  Object.defineProperty(type.prototype, 'low', {
    get: type.prototype.getLow,
    set: type.prototype.setLow,
  });

  type.prototype.getClose = function() {
    return typeof this._fields[4] == 'undefined' ? 0 : this._fields[4];
  }
  type.prototype.setClose = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearClose = function() {
    this._fields[4] = 0;
  };
  Object.defineProperty(type.prototype, 'close', {
    get: type.prototype.getClose,
    set: type.prototype.setClose,
  });

  type.prototype.getVolume = function() {
    return typeof this._fields[5] == 'undefined' ? 0 : this._fields[5];
  }
  type.prototype.setVolume = function(value) {
      this._fields[5] = value;
  };
  type.prototype.clearVolume = function() {
    this._fields[5] = 0;
  };
  Object.defineProperty(type.prototype, 'volume', {
    get: type.prototype.getVolume,
    set: type.prototype.setVolume,
  });

  type.prototype.getChange = function() {
    return typeof this._fields[6] == 'undefined' ? 0 : this._fields[6];
  }
  type.prototype.setChange = function(value) {
      this._fields[6] = value;
  };
  type.prototype.clearChange = function() {
    this._fields[6] = 0;
  };
  Object.defineProperty(type.prototype, 'change', {
    get: type.prototype.getChange,
    set: type.prototype.setChange,
  });

  return type;
})();

var DepthItem = (function() {
  var type = function(values) {
    this._fields = new Array(5);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof DepthItem) {
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

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(11));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setLevel(rawArray[0][0]);
      this.setB(rawArray[0][1]);
      this.setA(rawArray[0][2]);
      this.setBs(rawArray[0][3]);
      this.setAs(rawArray[0][4]);
    }
  };

  type.prototype.getLevel = function() {
    return typeof this._fields[0] == 'undefined' ? 0 : this._fields[0];
  }
  type.prototype.setLevel = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearLevel = function() {
    this._fields[0] = 0;
  };
  Object.defineProperty(type.prototype, 'level', {
    get: type.prototype.getLevel,
    set: type.prototype.setLevel,
  });

  type.prototype.getB = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setB = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearB = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'b', {
    get: type.prototype.getB,
    set: type.prototype.setB,
  });

  type.prototype.getA = function() {
    return typeof this._fields[2] == 'undefined' ? 0 : this._fields[2];
  }
  type.prototype.setA = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearA = function() {
    this._fields[2] = 0;
  };
  Object.defineProperty(type.prototype, 'a', {
    get: type.prototype.getA,
    set: type.prototype.setA,
  });

  type.prototype.getBs = function() {
    return typeof this._fields[3] == 'undefined' ? 0 : this._fields[3];
  }
  type.prototype.setBs = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearBs = function() {
    this._fields[3] = 0;
  };
  Object.defineProperty(type.prototype, 'bs', {
    get: type.prototype.getBs,
    set: type.prototype.setBs,
  });

  type.prototype.getAs = function() {
    return typeof this._fields[4] == 'undefined' ? 0 : this._fields[4];
  }
  type.prototype.setAs = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearAs = function() {
    this._fields[4] = 0;
  };
  Object.defineProperty(type.prototype, 'as', {
    get: type.prototype.getAs,
    set: type.prototype.setAs,
  });

  return type;
})();

var BookDelta = (function() {
  var type = function(values) {
    this._fields = new Array(8);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
  this._removes = new Array();
  this._fields[5] = new Array();
  if (values && values.removes && values.removes instanceof Array) {
    this.removes(values.removes);
  }
  this._level2tic = new Array();
  this._fields[6] = new Array();
  if (values && values.level2tic && values.level2tic instanceof Array) {
    this.level2tic(values.level2tic);
  }
  this._level1tic = new Array();
  this._fields[7] = new Array();
  if (values && values.level1tic && values.level1tic instanceof Array) {
    this.level1tic(values.level1tic);
  }
    if (values instanceof BookDelta) {
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

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(12));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setFantasyName(rawArray[0][0]);
      this.setSeqnum(rawArray[0][1]);
      this.setPlayerid(rawArray[0][2]);
      if (typeof this.newnew == 'undefined') {
        this.setNewnew({});
      }
      this._newnew._mergeFromRawArray(rawArray[0][3]);
      if (typeof this.ohlc == 'undefined') {
        this.setOhlc({});
      }
      this._ohlc._mergeFromRawArray(rawArray[0][4]);
      field = rawArray[0][5];
      if (field instanceof Array) {
        for (var i = 0; i < field.length; ++i) {
          if (typeof this._removes[i] == 'undefined') {
            var msg = new Order();
            this._removes[i] = msg;
            this._fields[5][i] = msg._raw;
          }
          this._removes[i]._mergeFromRawArray(field[i]);
        }
      }
      field = rawArray[0][6];
      if (field instanceof Array) {
        for (var i = 0; i < field.length; ++i) {
          if (typeof this._level2tic[i] == 'undefined') {
            var msg = new MarketTicker();
            this._level2tic[i] = msg;
            this._fields[6][i] = msg._raw;
          }
          this._level2tic[i]._mergeFromRawArray(field[i]);
        }
      }
      field = rawArray[0][7];
      if (field instanceof Array) {
        for (var i = 0; i < field.length; ++i) {
          if (typeof this._level1tic[i] == 'undefined') {
            var msg = new MarketTicker();
            this._level1tic[i] = msg;
            this._fields[7][i] = msg._raw;
          }
          this._level1tic[i]._mergeFromRawArray(field[i]);
        }
      }
    }
  };

  type.prototype.getFantasyName = function() {
    return typeof this._fields[0] == 'undefined' ? '' : this._fields[0];
  }
  type.prototype.setFantasyName = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearFantasyName = function() {
    this._fields[0] = '';
  };
  Object.defineProperty(type.prototype, 'fantasyName', {
    get: type.prototype.getFantasyName,
    set: type.prototype.setFantasyName,
  });

  type.prototype.getSeqnum = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setSeqnum = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearSeqnum = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'seqnum', {
    get: type.prototype.getSeqnum,
    set: type.prototype.setSeqnum,
  });

  type.prototype.getPlayerid = function() {
    return typeof this._fields[2] == 'undefined' ? '' : this._fields[2];
  }
  type.prototype.setPlayerid = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearPlayerid = function() {
    this._fields[2] = '';
  };
  Object.defineProperty(type.prototype, 'playerid', {
    get: type.prototype.getPlayerid,
    set: type.prototype.setPlayerid,
  });

  type.prototype.getNewnew = function() {
    return this._newnew;
  }
  type.prototype.setNewnew = function(value) {
    var msg = this._maybeConvertToMessage(OrderCore, value);
    this._newnew = msg;
    this._fields[3] = msg._raw;
  };
  type.prototype.clearNewnew = function() {
    this._fields[3] = undefined;
    this._newnew = undefined;
  };
  Object.defineProperty(type.prototype, 'newnew', {
    get: type.prototype.getNewnew,
    set: type.prototype.setNewnew,
  });

  type.prototype.getOhlc = function() {
    return this._ohlc;
  }
  type.prototype.setOhlc = function(value) {
    var msg = this._maybeConvertToMessage(ContractOHLC, value);
    this._ohlc = msg;
    this._fields[4] = msg._raw;
  };
  type.prototype.clearOhlc = function() {
    this._fields[4] = undefined;
    this._ohlc = undefined;
  };
  Object.defineProperty(type.prototype, 'ohlc', {
    get: type.prototype.getOhlc,
    set: type.prototype.setOhlc,
  });

    // Replacement setter
  type.prototype.setRemoves = function(values) {
    if (!(values instanceof Array)) {
      throw new TypeError();
    }
    this._fields[5].length = values.length;
     this._removes.length = values.length;
     for (var i = 0; i < values.length; ++i) {
       var msg = new Order(values[i]);
       this._removes[i] = msg;
       this._fields[5][i] = msg._raw;
     }
  };
  // Single value setter
  type.prototype.setRemovesAt = function(index, value) {
    var msg = this._maybeConvertToMessage(Order, value);
    this._removes[index] = msg;
    this._fields[5][index] = msg._raw;
  };
  // Getter
  type.prototype.getRemovesAt = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    if(this._fields[5].length < index) {
      throw new RangeError();
    }
    return this._removes[index];
  };
  type.prototype.getRemovesAsArray = function() {
    return this._removes.slice();
  };
  type.prototype.removes = function(indexOrValues, value) {
    if (typeof indexOrValues == 'undefined') {
      throw new TypeError('Not enough arguments');
    } else if (typeof indexOrValues == 'number') {
      if (typeof value == 'undefined') {
        return this.getRemovesAt(indexOrValues);
      } else {
        this.setRemovesAt(indexOrValues, value);
      }
    } else {
      this.setRemoves(indexOrValues);
    }
  };
  var removesCount = function() {
  console.assert(this._removes.length == this._fields[5].length);
    return this._removes.length;
  };
  Object.defineProperties(type.prototype, {
    removesCount: { get: removesCount },
    removesSize: { get: removesCount },
    removesLength: { get: removesCount },
  });
  type.prototype.getRemovesCount = removesCount;
  type.prototype.getRemovesSize = removesCount;
  type.prototype.getRemovesLength = removesCount;
  type.prototype.addRemoves = function(value) {
    if (typeof value == 'undefined') {
      throw new TypeError('Cannot add undefined.');
    }
    var msg = this._maybeConvertToMessage(Order, value);
    this._removes.push(msg);
    this._fields[5].push(msg._raw);
  console.assert(this._removes.length == this._fields[5].length);
  };
  type.prototype.removeRemoves = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    this._fields[5].splice(index, 1);
    this._removes.splice(index, 1);
  console.assert(this._removes.length == this._fields[5].length);
  };
  type.prototype.clearRemoves = function() {
    this._fields[5].length = 0;
    this._removes.length = 0;
  console.assert(this._removes.length == this._fields[5].length);
  };
    // Replacement setter
  type.prototype.setLevel2tic = function(values) {
    if (!(values instanceof Array)) {
      throw new TypeError();
    }
    this._fields[6].length = values.length;
     this._level2tic.length = values.length;
     for (var i = 0; i < values.length; ++i) {
       var msg = new MarketTicker(values[i]);
       this._level2tic[i] = msg;
       this._fields[6][i] = msg._raw;
     }
  };
  // Single value setter
  type.prototype.setLevel2ticAt = function(index, value) {
    var msg = this._maybeConvertToMessage(MarketTicker, value);
    this._level2tic[index] = msg;
    this._fields[6][index] = msg._raw;
  };
  // Getter
  type.prototype.getLevel2ticAt = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    if(this._fields[6].length < index) {
      throw new RangeError();
    }
    return this._level2tic[index];
  };
  type.prototype.getLevel2ticAsArray = function() {
    return this._level2tic.slice();
  };
  type.prototype.level2tic = function(indexOrValues, value) {
    if (typeof indexOrValues == 'undefined') {
      throw new TypeError('Not enough arguments');
    } else if (typeof indexOrValues == 'number') {
      if (typeof value == 'undefined') {
        return this.getLevel2ticAt(indexOrValues);
      } else {
        this.setLevel2ticAt(indexOrValues, value);
      }
    } else {
      this.setLevel2tic(indexOrValues);
    }
  };
  var level2ticCount = function() {
  console.assert(this._level2tic.length == this._fields[6].length);
    return this._level2tic.length;
  };
  Object.defineProperties(type.prototype, {
    level2ticCount: { get: level2ticCount },
    level2ticSize: { get: level2ticCount },
    level2ticLength: { get: level2ticCount },
  });
  type.prototype.getLevel2ticCount = level2ticCount;
  type.prototype.getLevel2ticSize = level2ticCount;
  type.prototype.getLevel2ticLength = level2ticCount;
  type.prototype.addLevel2tic = function(value) {
    if (typeof value == 'undefined') {
      throw new TypeError('Cannot add undefined.');
    }
    var msg = this._maybeConvertToMessage(MarketTicker, value);
    this._level2tic.push(msg);
    this._fields[6].push(msg._raw);
  console.assert(this._level2tic.length == this._fields[6].length);
  };
  type.prototype.removeLevel2tic = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    this._fields[6].splice(index, 1);
    this._level2tic.splice(index, 1);
  console.assert(this._level2tic.length == this._fields[6].length);
  };
  type.prototype.clearLevel2tic = function() {
    this._fields[6].length = 0;
    this._level2tic.length = 0;
  console.assert(this._level2tic.length == this._fields[6].length);
  };
    // Replacement setter
  type.prototype.setLevel1tic = function(values) {
    if (!(values instanceof Array)) {
      throw new TypeError();
    }
    this._fields[7].length = values.length;
     this._level1tic.length = values.length;
     for (var i = 0; i < values.length; ++i) {
       var msg = new MarketTicker(values[i]);
       this._level1tic[i] = msg;
       this._fields[7][i] = msg._raw;
     }
  };
  // Single value setter
  type.prototype.setLevel1ticAt = function(index, value) {
    var msg = this._maybeConvertToMessage(MarketTicker, value);
    this._level1tic[index] = msg;
    this._fields[7][index] = msg._raw;
  };
  // Getter
  type.prototype.getLevel1ticAt = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    if(this._fields[7].length < index) {
      throw new RangeError();
    }
    return this._level1tic[index];
  };
  type.prototype.getLevel1ticAsArray = function() {
    return this._level1tic.slice();
  };
  type.prototype.level1tic = function(indexOrValues, value) {
    if (typeof indexOrValues == 'undefined') {
      throw new TypeError('Not enough arguments');
    } else if (typeof indexOrValues == 'number') {
      if (typeof value == 'undefined') {
        return this.getLevel1ticAt(indexOrValues);
      } else {
        this.setLevel1ticAt(indexOrValues, value);
      }
    } else {
      this.setLevel1tic(indexOrValues);
    }
  };
  var level1ticCount = function() {
  console.assert(this._level1tic.length == this._fields[7].length);
    return this._level1tic.length;
  };
  Object.defineProperties(type.prototype, {
    level1ticCount: { get: level1ticCount },
    level1ticSize: { get: level1ticCount },
    level1ticLength: { get: level1ticCount },
  });
  type.prototype.getLevel1ticCount = level1ticCount;
  type.prototype.getLevel1ticSize = level1ticCount;
  type.prototype.getLevel1ticLength = level1ticCount;
  type.prototype.addLevel1tic = function(value) {
    if (typeof value == 'undefined') {
      throw new TypeError('Cannot add undefined.');
    }
    var msg = this._maybeConvertToMessage(MarketTicker, value);
    this._level1tic.push(msg);
    this._fields[7].push(msg._raw);
  console.assert(this._level1tic.length == this._fields[7].length);
  };
  type.prototype.removeLevel1tic = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    this._fields[7].splice(index, 1);
    this._level1tic.splice(index, 1);
  console.assert(this._level1tic.length == this._fields[7].length);
  };
  type.prototype.clearLevel1tic = function() {
    this._fields[7].length = 0;
    this._level1tic.length = 0;
  console.assert(this._level1tic.length == this._fields[7].length);
  };
  return type;
})();

var MarketQuote = (function() {
  var type = function(values) {
    this._fields = new Array(7);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof MarketQuote) {
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

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(13));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setBs(rawArray[0][0]);
      this.setB(rawArray[0][1]);
      this.setA(rawArray[0][2]);
      this.setAs(rawArray[0][3]);
      this.setL(rawArray[0][4]);
      this.setLs(rawArray[0][5]);
      this.setUdn(rawArray[0][6]);
    }
  };

  type.prototype.getBs = function() {
    return typeof this._fields[0] == 'undefined' ? 0 : this._fields[0];
  }
  type.prototype.setBs = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearBs = function() {
    this._fields[0] = 0;
  };
  Object.defineProperty(type.prototype, 'bs', {
    get: type.prototype.getBs,
    set: type.prototype.setBs,
  });

  type.prototype.getB = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setB = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearB = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'b', {
    get: type.prototype.getB,
    set: type.prototype.setB,
  });

  type.prototype.getA = function() {
    return typeof this._fields[2] == 'undefined' ? 0 : this._fields[2];
  }
  type.prototype.setA = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearA = function() {
    this._fields[2] = 0;
  };
  Object.defineProperty(type.prototype, 'a', {
    get: type.prototype.getA,
    set: type.prototype.setA,
  });

  type.prototype.getAs = function() {
    return typeof this._fields[3] == 'undefined' ? 0 : this._fields[3];
  }
  type.prototype.setAs = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearAs = function() {
    this._fields[3] = 0;
  };
  Object.defineProperty(type.prototype, 'as', {
    get: type.prototype.getAs,
    set: type.prototype.setAs,
  });

  type.prototype.getL = function() {
    return typeof this._fields[4] == 'undefined' ? 0 : this._fields[4];
  }
  type.prototype.setL = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearL = function() {
    this._fields[4] = 0;
  };
  Object.defineProperty(type.prototype, 'l', {
    get: type.prototype.getL,
    set: type.prototype.setL,
  });

  type.prototype.getLs = function() {
    return typeof this._fields[5] == 'undefined' ? 0 : this._fields[5];
  }
  type.prototype.setLs = function(value) {
      this._fields[5] = value;
  };
  type.prototype.clearLs = function() {
    this._fields[5] = 0;
  };
  Object.defineProperty(type.prototype, 'ls', {
    get: type.prototype.getLs,
    set: type.prototype.setLs,
  });

  type.prototype.getUdn = function() {
    return typeof this._fields[6] == 'undefined' ? 0 : this._fields[6];
  }
  type.prototype.setUdn = function(value) {
      this._fields[6] = value;
  };
  type.prototype.clearUdn = function() {
    this._fields[6] = 0;
  };
  Object.defineProperty(type.prototype, 'udn', {
    get: type.prototype.getUdn,
    set: type.prototype.setUdn,
  });

  return type;
})();

var MarketSnapshot = (function() {
  var type = function(values) {
    this._fields = new Array(5);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
  this._depth = new Array();
  this._fields[2] = new Array();
  if (values && values.depth && values.depth instanceof Array) {
    this.depth(values.depth);
  }
    if (values instanceof MarketSnapshot) {
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

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(14));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setSymbol(rawArray[0][0]);
      this.setWeek(rawArray[0][1]);
      field = rawArray[0][2];
      if (field instanceof Array) {
        for (var i = 0; i < field.length; ++i) {
          if (typeof this._depth[i] == 'undefined') {
            var msg = new DepthItem();
            this._depth[i] = msg;
            this._fields[2][i] = msg._raw;
          }
          this._depth[i]._mergeFromRawArray(field[i]);
        }
      }
      if (typeof this.ohlc == 'undefined') {
        this.setOhlc({});
      }
      this._ohlc._mergeFromRawArray(rawArray[0][3]);
      if (typeof this.quote == 'undefined') {
        this.setQuote({});
      }
      this._quote._mergeFromRawArray(rawArray[0][4]);
    }
  };

  type.prototype.getSymbol = function() {
    return typeof this._fields[0] == 'undefined' ? '' : this._fields[0];
  }
  type.prototype.setSymbol = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearSymbol = function() {
    this._fields[0] = '';
  };
  Object.defineProperty(type.prototype, 'symbol', {
    get: type.prototype.getSymbol,
    set: type.prototype.setSymbol,
  });

  type.prototype.getWeek = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setWeek = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearWeek = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'week', {
    get: type.prototype.getWeek,
    set: type.prototype.setWeek,
  });

    // Replacement setter
  type.prototype.setDepth = function(values) {
    if (!(values instanceof Array)) {
      throw new TypeError();
    }
    this._fields[2].length = values.length;
     this._depth.length = values.length;
     for (var i = 0; i < values.length; ++i) {
       var msg = new DepthItem(values[i]);
       this._depth[i] = msg;
       this._fields[2][i] = msg._raw;
     }
  };
  // Single value setter
  type.prototype.setDepthAt = function(index, value) {
    var msg = this._maybeConvertToMessage(DepthItem, value);
    this._depth[index] = msg;
    this._fields[2][index] = msg._raw;
  };
  // Getter
  type.prototype.getDepthAt = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    if(this._fields[2].length < index) {
      throw new RangeError();
    }
    return this._depth[index];
  };
  type.prototype.getDepthAsArray = function() {
    return this._depth.slice();
  };
  type.prototype.depth = function(indexOrValues, value) {
    if (typeof indexOrValues == 'undefined') {
      throw new TypeError('Not enough arguments');
    } else if (typeof indexOrValues == 'number') {
      if (typeof value == 'undefined') {
        return this.getDepthAt(indexOrValues);
      } else {
        this.setDepthAt(indexOrValues, value);
      }
    } else {
      this.setDepth(indexOrValues);
    }
  };
  var depthCount = function() {
  console.assert(this._depth.length == this._fields[2].length);
    return this._depth.length;
  };
  Object.defineProperties(type.prototype, {
    depthCount: { get: depthCount },
    depthSize: { get: depthCount },
    depthLength: { get: depthCount },
  });
  type.prototype.getDepthCount = depthCount;
  type.prototype.getDepthSize = depthCount;
  type.prototype.getDepthLength = depthCount;
  type.prototype.addDepth = function(value) {
    if (typeof value == 'undefined') {
      throw new TypeError('Cannot add undefined.');
    }
    var msg = this._maybeConvertToMessage(DepthItem, value);
    this._depth.push(msg);
    this._fields[2].push(msg._raw);
  console.assert(this._depth.length == this._fields[2].length);
  };
  type.prototype.removeDepth = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    this._fields[2].splice(index, 1);
    this._depth.splice(index, 1);
  console.assert(this._depth.length == this._fields[2].length);
  };
  type.prototype.clearDepth = function() {
    this._fields[2].length = 0;
    this._depth.length = 0;
  console.assert(this._depth.length == this._fields[2].length);
  };
  type.prototype.getOhlc = function() {
    return this._ohlc;
  }
  type.prototype.setOhlc = function(value) {
    var msg = this._maybeConvertToMessage(ContractOHLC, value);
    this._ohlc = msg;
    this._fields[3] = msg._raw;
  };
  type.prototype.clearOhlc = function() {
    this._fields[3] = undefined;
    this._ohlc = undefined;
  };
  Object.defineProperty(type.prototype, 'ohlc', {
    get: type.prototype.getOhlc,
    set: type.prototype.setOhlc,
  });

  type.prototype.getQuote = function() {
    return this._quote;
  }
  type.prototype.setQuote = function(value) {
    var msg = this._maybeConvertToMessage(MarketQuote, value);
    this._quote = msg;
    this._fields[4] = msg._raw;
  };
  type.prototype.clearQuote = function() {
    this._fields[4] = undefined;
    this._quote = undefined;
  };
  Object.defineProperty(type.prototype, 'quote', {
    get: type.prototype.getQuote,
    set: type.prototype.setQuote,
  });

  return type;
})();

