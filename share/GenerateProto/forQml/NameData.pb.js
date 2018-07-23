.pragma library
.import Protobuf 1.0 as Protobuf
.import 'ExData.pb.js' as Q__ExData__
'use strict';

var _file = {
  get descriptor() {
    if(!this._desc && Q__ExData__._file.descriptor) {
      this._desc = Protobuf.DescriptorPool.addFileDescriptor('Cg5OYW1lRGF0YS5wcm90bxIKZmFudGFzeWJpdBoMRXhEYXRhLnByb3RvIk8KDkZhbnRhc3lOYW1lQmFsEgwKBG5hbWUYCiABKAkSEgoKcHVibGljX2tleRgUIAEoCRIMCgRiaXRzGB4gASgEEg0KBXN0YWtlGCggASgDIjwKD0ZhbnRhc3lCaXRBd2FyZBIMCgRuYW1lGAogASgJEgwKBHByb2oYFCABKAUSDQoFYXdhcmQYHiABKAQiQQoNRmFudGFzeUJpdFBubBIjCgRzcG9zGBQgASgLMhUuZmFudGFzeWJpdC5TZXR0bGVQb3MSCwoDcG5sGB4gASgDIj4KDkZhbnRhc3lCaXRQcm9qEgwKBG5hbWUYCiABKAkSDAoEcHJvahgUIAEoBRIQCghwbGF5ZXJpZBgeIAEoCSJ4ChJHYW1lRmFudGFzeUJpdFByb2oSDgoGZ2FtZWlkGAogASgJEigKBGhvbWUYFCADKAsyGi5mYW50YXN5Yml0LkZhbnRhc3lCaXRQcm9qEigKBGF3YXkYHiADKAsyGi5mYW50YXN5Yml0LkZhbnRhc3lCaXRQcm9q', 438);
    }
    if (!this._desc)
      console.warn('Failed to initialize: NameData.proto');
    return this._desc;
  },
};

var FantasyNameBal = (function() {
  var type = function(values) {
    this._fields = new Array(4);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof FantasyNameBal) {
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
      this.setName(rawArray[0][0]);
      this.setPublicKey(rawArray[0][1]);
      this.setBits(rawArray[0][2]);
      this.setStake(rawArray[0][3]);
    }
  };

  type.prototype.getName = function() {
    return typeof this._fields[0] == 'undefined' ? '' : this._fields[0];
  }
  type.prototype.setName = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearName = function() {
    this._fields[0] = '';
  };
  Object.defineProperty(type.prototype, 'name', {
    get: type.prototype.getName,
    set: type.prototype.setName,
  });

  type.prototype.getPublicKey = function() {
    return typeof this._fields[1] == 'undefined' ? '' : this._fields[1];
  }
  type.prototype.setPublicKey = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearPublicKey = function() {
    this._fields[1] = '';
  };
  Object.defineProperty(type.prototype, 'publicKey', {
    get: type.prototype.getPublicKey,
    set: type.prototype.setPublicKey,
  });

  type.prototype.getBits = function() {
    return typeof this._fields[2] == 'undefined' ? 0 : this._fields[2];
  }
  type.prototype.setBits = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearBits = function() {
    this._fields[2] = 0;
  };
  Object.defineProperty(type.prototype, 'bits', {
    get: type.prototype.getBits,
    set: type.prototype.setBits,
  });

  type.prototype.getStake = function() {
    return typeof this._fields[3] == 'undefined' ? 0 : this._fields[3];
  }
  type.prototype.setStake = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearStake = function() {
    this._fields[3] = 0;
  };
  Object.defineProperty(type.prototype, 'stake', {
    get: type.prototype.getStake,
    set: type.prototype.setStake,
  });

  return type;
})();

var FantasyBitAward = (function() {
  var type = function(values) {
    this._fields = new Array(3);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof FantasyBitAward) {
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
      this.setName(rawArray[0][0]);
      this.setProj(rawArray[0][1]);
      this.setAward(rawArray[0][2]);
    }
  };

  type.prototype.getName = function() {
    return typeof this._fields[0] == 'undefined' ? '' : this._fields[0];
  }
  type.prototype.setName = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearName = function() {
    this._fields[0] = '';
  };
  Object.defineProperty(type.prototype, 'name', {
    get: type.prototype.getName,
    set: type.prototype.setName,
  });

  type.prototype.getProj = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setProj = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearProj = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'proj', {
    get: type.prototype.getProj,
    set: type.prototype.setProj,
  });

  type.prototype.getAward = function() {
    return typeof this._fields[2] == 'undefined' ? 0 : this._fields[2];
  }
  type.prototype.setAward = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearAward = function() {
    this._fields[2] = 0;
  };
  Object.defineProperty(type.prototype, 'award', {
    get: type.prototype.getAward,
    set: type.prototype.setAward,
  });

  return type;
})();

var FantasyBitPnl = (function() {
  var type = function(values) {
    this._fields = new Array(2);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof FantasyBitPnl) {
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
      if (typeof this.spos == 'undefined') {
        this.setSpos({});
      }
      this._spos._mergeFromRawArray(rawArray[0][0]);
      this.setPnl(rawArray[0][1]);
    }
  };

  type.prototype.getSpos = function() {
    return this._spos;
  }
  type.prototype.setSpos = function(value) {
    var msg = this._maybeConvertToMessage(Q__ExData__.SettlePos, value);
    this._spos = msg;
    this._fields[0] = msg._raw;
  };
  type.prototype.clearSpos = function() {
    this._fields[0] = undefined;
    this._spos = undefined;
  };
  Object.defineProperty(type.prototype, 'spos', {
    get: type.prototype.getSpos,
    set: type.prototype.setSpos,
  });

  type.prototype.getPnl = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setPnl = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearPnl = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'pnl', {
    get: type.prototype.getPnl,
    set: type.prototype.setPnl,
  });

  return type;
})();

var FantasyBitProj = (function() {
  var type = function(values) {
    this._fields = new Array(3);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof FantasyBitProj) {
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
      this.setName(rawArray[0][0]);
      this.setProj(rawArray[0][1]);
      this.setPlayerid(rawArray[0][2]);
    }
  };

  type.prototype.getName = function() {
    return typeof this._fields[0] == 'undefined' ? '' : this._fields[0];
  }
  type.prototype.setName = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearName = function() {
    this._fields[0] = '';
  };
  Object.defineProperty(type.prototype, 'name', {
    get: type.prototype.getName,
    set: type.prototype.setName,
  });

  type.prototype.getProj = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setProj = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearProj = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'proj', {
    get: type.prototype.getProj,
    set: type.prototype.setProj,
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

  return type;
})();

var GameFantasyBitProj = (function() {
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
    if (values instanceof GameFantasyBitProj) {
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
      this.setGameid(rawArray[0][0]);
      field = rawArray[0][1];
      if (field instanceof Array) {
        for (var i = 0; i < field.length; ++i) {
          if (typeof this._home[i] == 'undefined') {
            var msg = new FantasyBitProj();
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
            var msg = new FantasyBitProj();
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
       var msg = new FantasyBitProj(values[i]);
       this._home[i] = msg;
       this._fields[1][i] = msg._raw;
     }
  };
  // Single value setter
  type.prototype.setHomeAt = function(index, value) {
    var msg = this._maybeConvertToMessage(FantasyBitProj, value);
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
    var msg = this._maybeConvertToMessage(FantasyBitProj, value);
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
       var msg = new FantasyBitProj(values[i]);
       this._away[i] = msg;
       this._fields[2][i] = msg._raw;
     }
  };
  // Single value setter
  type.prototype.setAwayAt = function(index, value) {
    var msg = this._maybeConvertToMessage(FantasyBitProj, value);
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
    var msg = this._maybeConvertToMessage(FantasyBitProj, value);
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

